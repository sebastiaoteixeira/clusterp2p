#include <fcntl.h>
#include <stddef.h>
#include <sys/socket.h>

int set_socket_non_blocking(int socket)
{
    int flags = fcntl(socket, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl");
        return -1;
    }

    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl");
        return -1;
    }

    return 0;
}

int set_socket_blocking(int socket)
{
    int flags = fcntl(socket, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl");
        return -1;
    }

    if (fcntl(socket, F_SETFL, flags & ~O_NONBLOCK) == -1)
    {
        perror("fcntl");
        return -1;
    }

    return 0;
}

int sendall(int sockfd, const void *buf, size_t len, int flags)
{
    size_t total = 0;
    size_t bytesleft = len;
    int n;

    while (total < len)
    {
        n = send(sockfd, buf + total, bytesleft, flags);
        if (n == -1)
        {
            break;
        }
        total += n;
        bytesleft -= n;
    }

    return n == -1 ? -1 : 0;
}