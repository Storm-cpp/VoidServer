#include "logger.h"

Logger::Logger(const std::string& filename) {
    logFile.open(filename, std::ios::app);
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const std::string& message) {
    std::time_t now = std::time(nullptr);
    char timeStr[20];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    std::string logEntry = std::string(timeStr) + " - " + message + "\n";
    std::cout << logEntry;
    if (logFile.is_open()) {
        logFile << logEntry;
    }
}