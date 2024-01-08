#pragma once

#include <memory>
#include <arpa/inet.h>
#include <sys/socket.h>

class UDPSocketServer
{
public:
    explicit UDPSocketServer(int port);
    ~UDPSocketServer();
    int GetSocket() const;
private:
    int m_Socket;
};

class UDPSocketClient
{
public:
    UDPSocketClient() = default;

    [[ nodiscard ]] bool TryReadData(int server, std::string& data);
    void SendData(int server, const std::string& data, const sockaddr_in& sockAddr) const;
    sockaddr_in GetSockAddr() const;
private:
    sockaddr_in m_SockAddr = {};
    socklen_t m_SockAddrLen = sizeof(m_SockAddr);
};