#include "../app.hpp"



App::App(int argc, char *argv[]):
client_dto{ClientDTO()}
{
    this->check_arguments(argc);
    this->parse_arguments(argc, argv);

    SignalCatcher::get_instance()->bind(std::bind(&App::close, this, std::placeholders::_1));
}

App::~App(){
    this->close();
}

void App::close(std::int32_t reason){
    if(!this->is_running.load(std::memory_order_relaxed)) return;
    std::cout<< "[App] closing " << reason << std::endl;

    this->is_running.store(false, std::memory_order_relaxed);
    this->cv.notify_all();
}

void App::wait_for(){
    std::unique_lock<std::mutex> lock(this->mtx);
    this->cv.wait_for(lock, std::chrono::seconds(this->client_dto.get_connection_timeout()), [this](){
        return !this->is_running.load(std::memory_order_relaxed);
    });
}

void App::loop(){
    this->is_running.store(true, std::memory_order_relaxed);
    std::puts("[App] executing");

    while(this->is_running.load(std::memory_order_relaxed)){
        try{
            Client client = Client(this->client_dto.get_name(), this->client_dto.get_connection_timeout());
            client.connect(this->client_dto.get_port());
            client.send(this->client_dto.get_name());
            client.close();
        }catch(std::runtime_error error){

        }
        this->wait_for();
    }
}

void App::check_arguments(int argc){
    if(argc < 4){
        std::perror("Not enough arguments");
        std::exit(NOT_ENOUGH_ARGUMENTS);
    }
}
void App::parse_arguments(int argc, char *argv[]){
    this->client_dto.set_name(argv[1]);

    try{
        std::uint16_t port = std::stoi(argv[2]);
        this->client_dto.set_port(port);
    }catch(std::invalid_argument){
        std::perror("Port must be integer");
        std::exit(INVALID_PORT_TYPE);
    }

    try{
        std::uint64_t connection_timeout = std::stoi(argv[3]);
        this->client_dto.set_connection_timeout(connection_timeout);
    }catch(std::invalid_argument){
        std::perror("Connection timeout must be integer");
        std::exit(INVALID_CONNECTION_TIMEOUT_TYPE);
    }
}