#ifndef CONNECTION_CONTROLLER_H
#define CONNECTION_CONTROLLER_H

#include "../models/message.h"
#include "../protocol/protocol.h"
#include "../utils/logger.h"
#include <sys/socket.h>

// Function to process incoming connection and message
void process_connection(int client_socket, Logger& logger);

// Function to handle individual commands
std::string handle_command(const Command& cmd, Logger& logger);

#endif // CONNECTION_CONTROLLER_H