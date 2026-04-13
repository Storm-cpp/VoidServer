#include "server.h"
#include "../config/config.h"
#include "../config/config_loader.h"
#include "../controllers/connection_controller.h"
#include "../middleware/auth_middleware.h"
#include "../utils/logger.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>

int main() {
    // Load configuration from file
    Logger tempLogger("logs/server.log");
    ServerConfig config = ConfigLoader::loadFromFile("config/config.json", tempLogger);
    
    // Initialize logger with loaded config
    Logger logger(config.logFile);

    logger.log("═══════════════════════════════════════");
    logger.log("Starting VoidLink Server...");
    logger.log("Config - Host: " + config.host + ", Port: " + std::to_string(config.port));
    logger.log("═══════════════════════════════════════");

    int server_fd = initialize_server(config);
    if (server_fd < 0) {
        logger.log("❌ Failed to initialize server.");
        return EXIT_FAILURE;
    }

    logger.log("✓ Server initialized successfully.");

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    while (true) { // Main server loop
        logger.log("Waiting for new connection...");

        int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            logger.log("❌ Accept operation failed.");
            continue;
        }

        // Apply middleware
        if (!authenticate_client(new_socket)) {
            logger.log("❌ Authentication failed for client.");
            close(new_socket);
            continue;
        }

        // Handle connection with logger
        process_connection(new_socket, logger);
        close(new_socket);
    }

    cleanup_server(server_fd);
    logger.log("Server shut down.");
    return 0;
}