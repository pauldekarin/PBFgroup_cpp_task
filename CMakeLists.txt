# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(ServerClientConnections)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the output directory for build files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add subdirectories
add_subdirectory(common)  # Ensure common is added first
add_subdirectory(client)
add_subdirectory(server)
