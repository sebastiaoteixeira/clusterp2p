#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <sys/socket.h>
#include "../identity.h"

// server socket
int server_socket;


int init_node_listener();
int node_connect(struct sockaddr *to_connect, socklen_t to_connect_len);
int loop_node_listener();

#endif // NODE_H