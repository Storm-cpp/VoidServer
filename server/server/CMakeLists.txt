cmake_minimum_required(VERSION 3.10)
project(VoidLinkServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(src config protocol models controllers middleware database utils routes)
include_directories(../shared)

# Source files
set(SOURCES
    src/main.cpp
    src/server.cpp
    controllers/connection_controller.cpp
    middleware/auth_middleware.cpp
    utils/logger.cpp
    ../shared/utils/utils.cpp
)

# Create executable
add_executable(void_server ${SOURCES})