# Client CMakeLists
set(CLIENT_SRC_DIR ${CMAKE_SOURCE_DIR}/client/src)

# Define the client executable
add_executable(client
    ${CLIENT_SRC_DIR}/app.cpp
    ${CLIENT_SRC_DIR}/client.cpp
    ${CLIENT_SRC_DIR}/main.cpp
)

# Include directories for client headers
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/client)

# Link the common library to the client
target_link_libraries(client PRIVATE common)
