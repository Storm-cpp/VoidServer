#include "connection_controller.h"
#include <iostream>
#include <unistd.h>
#include <cstring>

void process_connection(int client_socket, Logger& logger) {
    char buffer[1024] = {0};
    
    // Read data from client
    memset(buffer, 0, 1024);
    ssize_t bytes_read = read(client_socket, buffer, 1024);
    
    if (bytes_read <= 0) {
        logger.log("Failed to read from client socket");
        return;
    }

    std::string input(buffer, bytes_read);
    logger.log("[RECEIVED] " + input);

    // Parse command
    Command cmd = ProtocolParser::parse(input);
    
    // Handle command
    std::string response = handle_command(cmd, logger);
    
    // Send response
    send(client_socket, response.c_str(), response.length(), 0);
    logger.log("[SENT] " + response);
}

std::string handle_command(const Command& cmd, Logger& logger) {
    switch (cmd.type) {
        case CommandType::PING:
            logger.log("PING received");
            return ProtocolParser::successResponse("PONG");
        
        case CommandType::USER_LOGIN:
            if (cmd.args.size() >= 2) {
                logger.log("LOGIN: " + cmd.args[0]);
                return ProtocolParser::successResponse("Logged in as " + cmd.args[0]);
            }
            return ProtocolParser::errorResponse("Invalid login format. Use: LOGIN:username:password");
        
        case CommandType::USER_REGISTER:
            if (cmd.args.size() >= 2) {
                logger.log("REGISTER: " + cmd.args[0]);
                return ProtocolParser::successResponse("User registered: " + cmd.args[0]);
            }
            return ProtocolParser::errorResponse("Invalid register format. Use: REGISTER:username:password");
        
        case CommandType::SEND_MESSAGE:
            if (cmd.args.size() >= 2) {
                logger.log("MESSAGE to " + cmd.args[0] + ": " + cmd.args[1]);
                return ProtocolParser::successResponse("Message sent to " + cmd.args[0]);
            }
            return ProtocolParser::errorResponse("Invalid message format. Use: MSG:channel:message");
        
        case CommandType::LIST_CHANNELS:
            logger.log("LIST_CHANNELS requested");
            return ProtocolParser::successResponse("Channels: general, random, announcements");
        
        case CommandType::JOIN_CHANNEL:
            if (cmd.args.size() >= 1) {
                logger.log("JOIN: " + cmd.args[0]);
                return ProtocolParser::successResponse("Joined channel: " + cmd.args[0]);
            }
            return ProtocolParser::errorResponse("Invalid join format. Use: JOIN:channel_name");
        
        case CommandType::CREATE_CHANNEL:
            if (cmd.args.size() >= 1) {
                logger.log("CREATE_CHANNEL: " + cmd.args[0]);
                return ProtocolParser::successResponse("Channel created: " + cmd.args[0]);
            }
            return ProtocolParser::errorResponse("Invalid create format. Use: CREATE:channel_name");
        
        case CommandType::UNKNOWN:
        default:
            logger.log("Unknown command: " + cmd.rawCommand);
            return ProtocolParser::errorResponse("Unknown command. Try: PING, LOGIN, REGISTER, MSG, CHANNELS, JOIN, CREATE");
    }
}