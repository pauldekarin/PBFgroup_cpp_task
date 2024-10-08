# Server CMakeLists
set(SERVER_SRC_DIR ${CMAKE_SOURCE_DIR}/server/src)

# Define the server executable
add_executable(server
    ${SERVER_SRC_DIR}/server.cpp
    ${SERVER_SRC_DIR}/main.cpp
)

# Include directories for server headers
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/server)

# Link the common library to the server
target_link_libraries(server PRIVATE common)
