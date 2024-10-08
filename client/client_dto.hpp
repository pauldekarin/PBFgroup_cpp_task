#ifndef CLIENT_DTO_HPP
#define CLIENT_DTO_HPP

#include <string>
#include <iostream>

class ClientDTO{
public:
    ClientDTO() = default;

    inline void set_name(const std::string &name)  noexcept{
        this->name = name;
    }
    inline void set_port(std::uint16_t port)  noexcept{
        this->port = port;
    }
    inline void set_connection_timeout(std::uint64_t connection_timeout)  noexcept{
        this->connection_timeout = connection_timeout;
    }

    inline std::string get_name() const noexcept{
        return this->name;
    }

    inline std::uint16_t get_port() const noexcept{
        return this->port;
    }

    inline std::uint64_t get_connection_timeout() const noexcept{
        return this->connection_timeout;
    }

private:
    std::string name;
    std::uint16_t port;
    std::uint64_t connection_timeout;
};

#endif