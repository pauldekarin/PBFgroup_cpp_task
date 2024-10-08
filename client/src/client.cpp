#include "../client.hpp"

Client::Client(std::string name, std::uint64_t connection_timeout):
name(name),
connection_timeout(connection_timeout)
{

}

Client::~Client(){
    this->close();
}

Client::Client(Client &&other) noexcept{

}

Client& Client::operator=(Client &&other) noexcept{
    return *this;
}

void Client::connect(std::uint16_t server_port){
    this->is_run.store(true, std::memory_order_relaxed);

    std::int32_t sockfd;
    std::int32_t connection_attempt = 1;
    std::int32_t retrying_timeout = 1000;

    std::chrono::duration<float> total_time{0.0f};

    while(this->is_run.load(std::memory_order_relaxed)){
        std::chrono::steady_clock::time_point begin_time = std::chrono::high_resolution_clock::now();

        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            std::perror("[Client] connect: can't create");
            this->is_run.store(false, std::memory_order_relaxed);
            throw std::runtime_error("socket()");
        }

        sockaddr_in s_addr;
        std::memset(&s_addr, 0, sizeof(s_addr));
        s_addr.sin_family = AF_INET;
        s_addr.sin_port = htons(server_port);
        s_addr.sin_addr.s_addr = INADDR_ANY;
        
        std::cout << "[Client] connect: trying connect " << connection_attempt++ << "..." << std::endl;

        if(::connect(sockfd, reinterpret_cast<sockaddr*>(&s_addr), sizeof(s_addr)) < 0){
            std::perror("[Client] connect: can't connect");

            ::close(sockfd);

            if(!this->enabled_retries.load(std::memory_order_relaxed)){
                throw std::runtime_error("connect()");
            }
            if(connection_attempt == 2){
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }else{
                std::this_thread::sleep_for(std::chrono::milliseconds(
                    std::min(retrying_timeout, static_cast<int>(this->connection_timeout * 1000 - std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count()))
                ));
            }

        }else{
            this->is_connect.store(true, std::memory_order_relaxed);
            this->client_fd = sockfd;

            std::puts("[Client] connect: successfully");
            break;
        }

        total_time += std::chrono::high_resolution_clock::now() - begin_time;

        if(std::chrono::duration_cast<std::chrono::seconds>(total_time).count() >= this->connection_timeout){
            std::perror("[Client] connect: Connection timeout");
            this->is_run.store(false, std::memory_order_relaxed);
            throw std::runtime_error("Connection timeout");
        }
    }
}

ssize_t Client::send(const char *buff) const noexcept{
    if(!this->is_running()){
        std::perror("[Client] send: client is not connected");
        return 0;
    }

    ssize_t sended_bytes = ::send(this->client_fd, buff, std::strlen(buff), 0);
    
    if(sended_bytes > 0){
        std::cout << "[Client] send: Message: '" << buff << "' -- Bytes: " << sended_bytes << std::endl;
    }else{
        std::perror("[Client] send: can't send");
    }
    
    return sended_bytes;
}


void Client::close(){
    if(!this->is_connected()) return;
    this->is_connect.store(false, std::memory_order_relaxed);
    
    std::puts("[Client] closed");
    ::close(this->client_fd);
}

std::string Client::get_name() const noexcept{
    std::lock_guard<std::mutex> lock(this->name_mutex);
    return this->name;
}