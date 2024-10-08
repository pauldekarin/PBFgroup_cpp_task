#include <iostream>
#include <string>
#include "../server.hpp"

#define PORT_UNAVALIABLE 1
#define INCORRECT_PORT_TYPE 2


int main(int argc, char *argv[]){
    if(argc <= 1){
        std::puts("PORT is not defined");
        std::exit(PORT_UNAVALIABLE);
    }

    try{
        Server server = Server();
        std::uint32_t port = std::stoi(argv[1]);
        server.listen_on_port(port);

        while(server.is_listening());
        
    }catch(std::invalid_argument){
        std::puts("PORT must be 'int'");
        std::exit(INCORRECT_PORT_TYPE);
    }
}