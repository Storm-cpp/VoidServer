#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include "config.h"
#include "../utils/logger.h"

class ConfigLoader {
public:
    static ServerConfig loadFromFile(const std::string& filepath, Logger& logger) {
        ServerConfig config;
        std::ifstream file(filepath);
        
        if (!file.is_open()) {
            logger.log("Config file not found: " + filepath + ", using defaults");
            return config;
        }

        std::string line;
        std::string section;
        
        while (std::getline(file, line)) {
            // Trim whitespace
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            
            // Skip empty lines and comments
            if (line.empty() || line[0] == '#' || line[0] == ';') {
                continue;
            }

            // Parse JSON-like format (simplified)
            if (line.find("\"port\"") != std::string::npos) {
                config.port = parseIntValue(line);
            }
            else if (line.find("\"host\"") != std::string::npos) {
                config.host = parseStringValue(line);
            }
            else if (line.find("\"backlog\"") != std::string::npos) {
                config.backlog = parseIntValue(line);
            }
            else if (line.find("\"file\"") != std::string::npos) {
                config.logFile = parseStringValue(line);
            }
            else if (line.find("\"debug\"") != std::string::npos) {
                config.debug = parseBoolValue(line);
            }
            else if (line.find("\"enabled\"") != std::string::npos && line.find("auth") != std::string::npos) {
                config.authEnabled = parseBoolValue(line);
            }
        }

        file.close();
        logger.log("Config loaded successfully from: " + filepath);
        return config;
    }

private:
    static int parseIntValue(const std::string& line) {
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::string value = line.substr(pos + 1);
            value.erase(0, value.find_first_not_of(" \t:"));
            value.erase(value.find_last_not_of(" \t,\r\n") + 1);
            try {
                return std::stoi(value);
            } catch (...) {
                return 0;
            }
        }
        return 0;
    }

    static std::string parseStringValue(const std::string& line) {
        size_t start = line.find('"');
        if (start == std::string::npos) return "";
        start++;
        size_t end = line.find('"', start);
        if (end == std::string::npos) return "";
        return line.substr(start, end - start);
    }

    static bool parseBoolValue(const std::string& line) {
        return line.find("true") != std::string::npos;
    }
};

#endif // CONFIG_LOADER_H
