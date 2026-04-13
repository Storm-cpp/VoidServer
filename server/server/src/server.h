#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "../config/config.h"

// Function declarations
int initialize_server(ServerConfig config);
void handle_connection(int server_fd, struct sockaddr_in address, int addrlen);
void cleanup_server(int server_fd);

#endif // SERVER_H