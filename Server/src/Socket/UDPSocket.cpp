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

    std::cout << "Server running on port: " << port << std::endl;
}

UDPSocketServer::~UDPSocketServer()
{
    close(m_Socket);
}

int UDPSocketServer::operator()() const
{
    return m_Socket;
}

// Client
std::string UDPSocketClient::WaitForData(int server)
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    if (recvfrom(server, buffer, sizeof(buffer), 0, (sockaddr*)&m_SockAddr, &m_SockAddrLen) <= 0)
    {
        std::cerr << "recfrom() failed with error code: " << errno << std::endl;
    }

    std::string output = buffer;
    std::erase(output, '\n');

    return output;
}

void UDPSocketClient::SendData(int server, const std::string& data)
{
    if (sendto(server, data.c_str(), data.size(), 0, (sockaddr*)&m_SockAddr, m_SockAddrLen) <= 0)
    {
        std::cerr << "sendto() failed with error code: " << errno << std::endl;
    }
}