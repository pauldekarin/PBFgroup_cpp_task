#ifndef APP_HPP
#define APP_HPP

#define NOT_ENOUGH_ARGUMENTS 1
#define INVALID_PORT_TYPE 2 
#define INVALID_CONNECTION_TIMEOUT_TYPE 3

#include "client.hpp"
#include "client_dto.hpp"
#include "../common/signal_catcher.hpp"

class App{
public:
    App(int argc, char *argv[]);
    ~App();

    void loop();
    void close(std::int32_t reason = 0);

private:
    ClientDTO client_dto;
    std::atomic<bool> is_running{false};

    std::mutex mtx;
    std::condition_variable cv;

    void check_arguments(int argc);
    void parse_arguments(int argc, char *argv[]);
    void wait_for();
    
};

#endif