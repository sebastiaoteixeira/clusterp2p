#include <arpa/inet.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "node.h"
#include "shared.h"
#include "../identity.h"
#include "../utils.h"
#include "../logger.h"

static char *HOST;
static int PORT;
static int MAX_CONNECTIONS;

// sockets create
int init_node_listener(char* host, int port, int max_connections)
{
    HOST = host;
    PORT = port;
    MAX_CONNECTIONS = max_connections;

    // Init server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("socket");
        return -1;
    }

    // Reuse server socket
    int reuse = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
    {
        perror("setsockopt");
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(HOST);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("bind");
        return -1;
    }

    if (listen(server_socket, 5) == -1)
    {
        perror("listen");
        return -1;
    }

    char server_ip[INET_ADDRSTRLEN];
    struct ifaddrs *ifaddr, *ifa;
    // Print listening IPs and port (if 0.0.0.0 print all)
    if (server_address.sin_addr.s_addr == INADDR_ANY && getifaddrs(&ifaddr) != -1)
    {

        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr == NULL)
            {
                continue;
            }

            if (ifa->ifa_addr->sa_family == AF_INET)
            {
                struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
                inet_ntop(AF_INET, &(addr->sin_addr), server_ip, INET_ADDRSTRLEN);
                info("Listening on %s:%d", server_ip, ntohs(server_address.sin_port));
            }
        }

        freeifaddrs(ifaddr);
    }
    else
    {
        inet_ntop(AF_INET, &(server_address.sin_addr), server_ip, INET_ADDRSTRLEN);
        info("Listening on %s:%d", server_ip, ntohs(server_address.sin_port));
    }

    // Non-blocking server socket
    set_socket_non_blocking(server_socket);

    return 0;
}

// sockets connect
int node_connect(struct sockaddr *to_connect, socklen_t to_connect_len)
{
    if (connections_full())
    {
        return -1;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket");
        return -1;
    }
    
    if (connect(sockfd, to_connect, to_connect_len) == -1)
    {
        if (errno != EINPROGRESS)
        {
            perror("connect");
            return -1;
        }
    }

    add_connection(server_socket);
}

int loop_node_listener()
{
    debug("Creating epoll");
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        perror("epoll_create1");
        return -1;
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_socket;

    debug("Adding server socket to epoll");
    

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &event) == -1)
    {
        perror("epoll_ctl");
        close(epoll_fd);
        return -1;
    }

    struct epoll_event *events = malloc(sizeof(struct epoll_event) * MAX_CONNECTIONS);
    if (events == NULL)
    {
        perror("malloc");
        close(epoll_fd);
        return -1;
    }    

    while (1)
    {
        int n = epoll_wait(epoll_fd, events, MAX_CONNECTIONS, -1);
        if (n == -1)
        {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < n; i++)
        {
            if (events[i].data.fd == server_socket)
            {
                struct sockaddr_in peer;
                socklen_t peer_len = sizeof(peer);
                int client_socket = accept(server_socket, (struct sockaddr *)&peer, &peer_len);
                
                if (client_socket == -1)
                {
                    perror("accept");
                    continue;
                }

                set_socket_non_blocking(client_socket);

                event.events = EPOLLIN | EPOLLET;
                event.data.fd = client_socket;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event) == -1)
                {
                    perror("epoll_ctl");
                    close(client_socket);
                    continue;
                }

                char peer_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(peer.sin_addr), peer_ip, INET_ADDRSTRLEN);
                info("New connection from %s:%d", peer_ip, ntohs(peer.sin_port));
            }
            else
            {
                char buf[1024];
                int bytes_read = recv(events[i].data.fd, buf, sizeof(buf), 0);
                struct sockaddr_in peer;
                socklen_t peer_len = sizeof(peer);
                getpeername(events[i].data.fd, (struct sockaddr *)&peer, &peer_len);
                
                if (bytes_read == -1)
                {
                    perror("recv");
                    close(events[i].data.fd);
                    continue;
                }

                if (bytes_read == 0)
                {
                    char peer_ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(peer.sin_addr), peer_ip, INET_ADDRSTRLEN);
                    info("Connection closed from %s:%d", peer_ip, ntohs(peer.sin_port));

                    close(events[i].data.fd);
                    continue;
                }

                char peer_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(peer.sin_addr), peer_ip, INET_ADDRSTRLEN);
                debug("Received %d bytes from %s:%d", bytes_read, peer_ip, ntohs(peer.sin_port));

                // Send response
                if (send(events[i].data.fd, buf, bytes_read, 0) == -1)
                {
                    perror("send");
                    close(events[i].data.fd);
                    continue;
                }

                debug("Sent %d bytes to %s:%d", bytes_read, peer_ip, ntohs(peer.sin_port));
            }
        }
    }

    return 0;
}

// sockets send
int node_send(int sockfd, const void *buf, size_t len, int flags)
{
    return send(sockfd, buf, len, flags);
}