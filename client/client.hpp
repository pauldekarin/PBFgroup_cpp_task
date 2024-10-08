#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <atomic>
#include <mutex>
#include <chrono>
#include <thread>
#include <iostream>
#include <cmath>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>


class Client{
public:
    Client(std::string name = "", std::uint64_t connection_timeout = 5);
    ~Client();

    Client(const Client& other) = delete;
    Client& operator=(const Client& other) = delete;

    Client(Client &&other) noexcept;
    Client& operator=(Client &&other) noexcept;

    inline std::uint16_t get_port() const noexcept { return this->port.load(std::memory_order_relaxed); }
    inline std::uint64_t get_connection_timeout() const noexcept { return this->connection_timeout.load(std::memory_order_relaxed); }
    inline bool is_running() const noexcept { return this->is_run.load(std::memory_order_relaxed); }
    inline bool is_connected() const noexcept { return this->is_connect.load(std::memory_order_relaxed); }
    inline void enable_retries(bool t = true) noexcept { this->enabled_retries.store(t, std::memory_order_relaxed); }
    std::string get_name() const noexcept;

    void connect(std::uint16_t server_port);
    ssize_t send(const char *buff) const noexcept;
    inline ssize_t send(const std::string &msg) const noexcept { return this->send(msg.c_str()); }
    void close();

private:
    std::string name;
    mutable std::mutex name_mutex;

    std::int32_t client_fd;

    std::atomic<std::uint16_t> port;
    std::atomic<std::uint64_t> connection_timeout;
    std::atomic<bool> is_run{false};
    std::atomic<bool> is_connect{false};
    std::atomic<bool> enabled_retries{false};
};

#endif