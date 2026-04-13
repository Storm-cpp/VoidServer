#include "server.h"
#include <iostream>
#include <unistd.h>
#include <cstring>

int initialize_server(ServerConfig config) {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    // Initialize socket (AF_INET for IPv4, SOCK_STREAM for TCP)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Configure socket options for immediate port reuse
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setsockopt failed");
        close(server_fd);
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Bind to all network interfaces
    address.sin_port = htons(config.port); // Listening port

    // Bind socket to the specified port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind operation failed");
        close(server_fd);
        return -1;
    }

    // Start listening for incoming connections
    if (listen(server_fd, config.backlog) < 0) {
        perror("Listen operation failed");
        close(server_fd);
        return -1;
    }

    std::cout << "VoidLink Server started on port " << config.port << "." << std::endl;
    return server_fd;
}

void handle_connection(int server_fd, struct sockaddr_in address, int addrlen) {
    int new_socket;
    char buffer[1024] = {0};
    const char* hello = "Connection established. Welcome to VoidLink messaging server.";

    while (true) { // Main server loop for handling multiple connections
        std::cout << "\n[LOG] Waiting for new connection..." << std::endl;

        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept operation failed");
            continue; // Continue accepting other connections on error
        }

        memset(buffer, 0, 1024); // Clear buffer for new message
        read(new_socket, buffer, 1024);
        std::cout << "[RECEIVED] " << buffer << std::endl;

        send(new_socket, hello, strlen(hello), 0);
        std::cout << "[SENT] Response sent." << std::endl;

        close(new_socket); // Close connection with this client and proceed to next
    }
}

void cleanup_server(int server_fd) {
    close(server_fd);
}