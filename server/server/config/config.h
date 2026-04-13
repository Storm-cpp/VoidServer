#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct ServerConfig {
    int port = 8080;
    int backlog = 5;
    std::string host = "0.0.0.0";
    std::string logFile = "logs/server.log";
    bool debug = true;
    bool authEnabled = true;
    // Add more config options as needed
};

#endif // CONFIG_H