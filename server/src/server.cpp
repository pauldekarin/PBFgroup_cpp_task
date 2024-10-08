#include "../server.hpp"

Server::Server()
{
    SignalCatcher::get_instance()->bind(std::bind(&Server::stop_listen, this, std::placeholders::_1));

    this->logger = std::make_unique<Logger>();
}

Server::~Server(){

}

Server::Server(Server &&other) noexcept{
    if(this == &other) return;

    if(other.is_listening() && this->is_listening() && other.get_port() != this->get_port()){
        this->stop_listen(0);
    }
}

Server& Server::operator=(Server &&other) noexcept{
    if(this != &other){
        this->is_listen.store(other.is_listening(), std::memory_order_relaxed);

        other.stop_listen(0);
    }

    return *this;
}

void Server::stop_listen(std::int32_t reason){
    if(!this->is_listening()) return;
    std::cout << "[Server] stop listen" << std::endl;
    this->is_listen.store(false, std::memory_order_relaxed);

    if(this->listen_thread.joinable()){
        this->listen_thread.join();
    }

    close(this->sockfd);
}

void Server::configure(std::uint16_t port){
    std::cout << "[Server] Configure server on PORT: " << port << std::endl;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0){
        std::perror("[Server] (listen): can't create socket");
        throw std::runtime_error("socket()");
    }
    sockaddr_in s_addr;
    std::memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = INADDR_ANY;
    s_addr.sin_port = htons(port);


    if(bind(sockfd, reinterpret_cast<sockaddr*>(&s_addr), sizeof(s_addr)) < 0){
        std::perror("[Server] listen: cant bind");
        close(sockfd);
        throw std::runtime_error("bind()");
    }

    if(::listen(sockfd, SOMAXCONN) < 0){
        std::perror("[Server] listen: can' listen");
        close(sockfd);
        throw std::runtime_error("listen()");
    }

    this->sockfd = sockfd;
    this->port = port;
}

void Server::accept_client(){
    std::lock_guard<std::mutex> lock(this->data_mutex);

    sockaddr_in s_addr_client;
    socklen_t s_len_client;

    int clientfd = accept(sockfd, reinterpret_cast<sockaddr*>(&s_addr_client), &s_len_client);
    
    if(clientfd < 0){
        std::perror("[Server] listen: Accept error");
        return;
    }

    std::cout << "[Server] listen: New connection " << clientfd << std::endl;
    this->connected_clients.push_back(clientfd);
}

void Server::read_message(fd_set &readfds){
    for(auto it = this->connected_clients.begin(); it != this->connected_clients.end();){
        if(FD_ISSET(*it, &readfds)){
            char buffer[1024];
            std::int32_t read_bytes = recv(*it, buffer, sizeof(buffer), 0);

            if(read_bytes == 0){
                std::cout << "[Server] Client disconnected " << *it << std::endl;
                it = this->connected_clients.erase(it);
                continue;
            }else{
                std::cout << "[Server] Client " << *it << " received message: " << buffer << std::endl;
                this->logger->write(buffer);
            }
        }

        it++;
    }
}

void Server::listen(){
    std::cout << "[Server] Listening on PORT: " << this->port << std::endl;

    while(this->is_listening()){
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        std::int32_t maxfd = sockfd;

        for(auto it = this->connected_clients.begin(); it != this->connected_clients.end(); it++){
            maxfd = std::max(maxfd, *it);
            FD_SET(*it, &readfds);
        }

        int r = select(maxfd + 1, &readfds, nullptr, nullptr, nullptr);
        if(r < 0){

        }else if(r == 0){

        }else{
            if(FD_ISSET(sockfd, &readfds)){
                this->accept_client();
            }
            this->read_message(readfds);
        }
    }
}
void Server::listen_on_port(std::uint16_t port){
    this->is_listen.store(true, std::memory_order_relaxed);
    this->configure(port);
    this->listen_thread = std::thread(&Server::listen, this);
    this->listen_thread.detach();
}

