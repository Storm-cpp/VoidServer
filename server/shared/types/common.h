#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <string>

enum class MessageType {
    TEXT,
    IMAGE,
    FILE
};

struct User {
    std::string id;
    std::string username;
    // Add more fields
};

struct Channel {
    std::string id;
    std::string name;
    // Add more fields
};

#endif // COMMON_TYPES_H