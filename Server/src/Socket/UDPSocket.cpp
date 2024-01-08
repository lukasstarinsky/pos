#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <iostream>
#include "UDPSocket.hpp"

UDPSocketServer::UDPSocketServer(int port)
{
    m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_Socket == -1)
    {
        throw std::runtime_error("socket() failed.\n");
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(m_Socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        throw std::runtime_error("bind() failed.\n");
    }

    timeval readTimeout;
    readTimeout.tv_sec = 0;
    readTimeout.tv_usec = 1;
    if (setsockopt(m_Socket, SOL_SOCKET, SO_RCVTIMEO, &readTimeout, sizeof(readTimeout)) == -1)
    {
        throw std::runtime_error("setsockopt() failed.\n");
    }

    if (setsockopt(m_Socket, SOL_SOCKET, SO_SNDTIMEO, &readTimeout, sizeof(readTimeout)) == -1)
    {
        throw std::runtime_error("setsockopt() failed.\n");
    }

    std::cout << "Server running on port: " << port << std::endl;
}

UDPSocketServer::~UDPSocketServer()
{
    close(m_Socket);
}

int UDPSocketServer::GetSocket() const
{
    return m_Socket;
}

// Client
bool UDPSocketClient::TryReadData(int server, std::string& data, bool peek)
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int flags = peek ? MSG_PEEK : 0;
    if (recvfrom(server, buffer, sizeof(buffer), flags, (sockaddr*)&m_SockAddr, &m_SockAddrLen) <= 0)
    {
        return false;
    }

    std::string output = buffer;
    std::erase(output, '\n');
    data = output;
    return true;
}

void UDPSocketClient::SendData(int server, const std::string& data, const sockaddr_in& sockAddr) const
{
    if (sendto(server, data.c_str(), data.size(), 0, (sockaddr*)&sockAddr, m_SockAddrLen) <= 0)
    {
//        std::cerr << "sendto() failed with error code: " << errno << std::endl;
    }
}

sockaddr_in UDPSocketClient::GetSockAddr() const
{
    return m_SockAddr;
}