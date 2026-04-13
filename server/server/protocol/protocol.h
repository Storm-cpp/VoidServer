#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <vector>
#include <sstream>

enum class CommandType {
    PING,
    USER_LOGIN,
    USER_REGISTER,
    SEND_MESSAGE,
    GET_MESSAGES,
    LIST_CHANNELS,
    JOIN_CHANNEL,
    LEAVE_CHANNEL,
    CREATE_CHANNEL,
    UNKNOWN
};

struct Command {
    CommandType type;
    std::vector<std::string> args;
    std::string rawCommand;
};

class ProtocolParser {
public:
    // Parse incoming command from client
    // Format: "COMMAND:arg1:arg2:arg3"
    // Example: "PING", "USER_LOGIN:username:password123", "SEND_MESSAGE:channel:hello world"
    
    static Command parse(const std::string& input) {
        Command cmd;
        cmd.rawCommand = input;
        cmd.type = CommandType::UNKNOWN;

        if (input.empty()) {
            return cmd;
        }

        // Split by colon
        std::vector<std::string> parts = split(input, ':');
        
        if (parts.empty()) {
            return cmd;
        }

        std::string commandName = parts[0];
        
        // Convert to uppercase for comparison
        for (auto& c : commandName) {
            c = std::toupper(c);
        }

        // Identify command type
        if (commandName == "PING") {
            cmd.type = CommandType::PING;
        }
        else if (commandName == "LOGIN") {
            cmd.type = CommandType::USER_LOGIN;
            if (parts.size() >= 3) {
                cmd.args.push_back(parts[1]); // username
                cmd.args.push_back(parts[2]); // password
            }
        }
        else if (commandName == "REGISTER") {
            cmd.type = CommandType::USER_REGISTER;
            if (parts.size() >= 3) {
                cmd.args.push_back(parts[1]); // username
                cmd.args.push_back(parts[2]); // password
            }
        }
        else if (commandName == "MSG" || commandName == "SEND_MESSAGE") {
            cmd.type = CommandType::SEND_MESSAGE;
            if (parts.size() >= 3) {
                cmd.args.push_back(parts[1]); // channel
                cmd.args.push_back(parts[2]); // message
            }
        }
        else if (commandName == "MESSAGES" || commandName == "GET_MESSAGES") {
            cmd.type = CommandType::GET_MESSAGES;
            if (parts.size() >= 2) {
                cmd.args.push_back(parts[1]); // channel
            }
        }
        else if (commandName == "CHANNELS" || commandName == "LIST_CHANNELS") {
            cmd.type = CommandType::LIST_CHANNELS;
        }
        else if (commandName == "JOIN") {
            cmd.type = CommandType::JOIN_CHANNEL;
            if (parts.size() >= 2) {
                cmd.args.push_back(parts[1]); // channel name
            }
        }
        else if (commandName == "LEAVE") {
            cmd.type = CommandType::LEAVE_CHANNEL;
            if (parts.size() >= 2) {
                cmd.args.push_back(parts[1]); // channel name
            }
        }
        else if (commandName == "CREATE") {
            cmd.type = CommandType::CREATE_CHANNEL;
            if (parts.size() >= 2) {
                cmd.args.push_back(parts[1]); // channel name
            }
        }

        return cmd;
    }

    // Generate response to send back to client
    static std::string formatResponse(const std::string& status, const std::string& message) {
        // Format: "STATUS:MESSAGE"
        return status + ":" + message;
    }

    static std::string successResponse(const std::string& message = "OK") {
        return formatResponse("SUCCESS", message);
    }

    static std::string errorResponse(const std::string& message) {
        return formatResponse("ERROR", message);
    }

private:
    static std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;

        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }

        return tokens;
    }
};

#endif // PROTOCOL_H
