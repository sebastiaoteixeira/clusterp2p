#ifndef UTILS_H
#define UTILS_H

int set_socket_non_blocking(int socket);

int set_socket_blocking(int socket);

int sendall(int sockfd, const void *buf, size_t len, int flags);

#endif // UTILS_H