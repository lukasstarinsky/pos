#pragma once

#include <memory>

class Socket
{
public:
    Socket(const char* address, int port);
    ~Socket();
    void SendData(const std::string& data) const;

    static std::unique_ptr<Socket> CreateConnection(const char* address, int port);
};