#include <iostream>
#include <string>

#include "../../common/signal_catcher.hpp"
#include "../client.hpp"


#define NOT_ENOUGH_ARGUMENTS 1
#define INVALID_PORT_TYPE 2 
#define INVALID_CONNECTION_TIMEOUT_TYPE 3

#include "../app.hpp"

int main(int argc, char *argv[]){
    App app{argc, argv};
    app.loop();
    
    std::exit(EXIT_SUCCESS);
}