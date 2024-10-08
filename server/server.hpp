#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include <memory>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#include "../common/signal_catcher.hpp"
#include "../common/logger.hpp"

class Server{
public:
    Server();
    ~Server();

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    
    Server(Server&& other) noexcept;
    Server& operator=(Server&& other) noexcept;

    void listen_on_port(std::uint16_t port = 3000);
    void stop_listen(std::int32_t reason);

    inline bool is_listening() { return this->is_listen.load(std::memory_order_relaxed); }
    inline std::uint16_t get_port() { return this->port; }

private:
    std::atomic<bool> is_listen{false};
    std::unique_ptr<Logger> logger;

    std::vector<std::int32_t> connected_clients;

    std::uint16_t port;
    std::int32_t sockfd;

    void configure(std::uint16_t port);
    void listen();
    void accept_client();
    void read_message(fd_set &readfds);
    void handle_disconnect(std::int32_t clientfd);

    std::thread listen_thread;

    std::mutex data_mutex;
};

#endif