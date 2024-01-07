#pragma once

#include <memory>

class Socket
{
public:
    Socket(const char* address, int port);
    ~Socket();
    bool TrySendData(const std::string& data) const;
    bool TryReadData(std::string& output, char delimiter = ';') const;

    static std::unique_ptr<Socket> CreateConnection(const char* address, int port);
};