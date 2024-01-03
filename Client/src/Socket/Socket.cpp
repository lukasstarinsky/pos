#include <iostream>
#include "Socket.hpp"

#ifdef _WIN64
#include <winsock2.h>
#include <ws2tcpip.h>

namespace
{
    WSADATA g_WSAData;
    SOCKET g_SocketHandle = INVALID_SOCKET;
}

Socket::Socket(const char* address, int port)
{
    if (WSAStartup(MAKEWORD(2, 2), &g_WSAData) != 0)
    {
        throw std::runtime_error("Failed to startup WSA");
    }

    addrinfo addressInfoHints = {};
    addressInfoHints.ai_protocol = IPPROTO_TCP;
    addressInfoHints.ai_family = AF_INET;
    addressInfoHints.ai_socktype = SOCK_STREAM;

    addrinfo* addressInfo = nullptr;
    if (getaddrinfo(address, std::to_string(port).c_str(), &addressInfoHints, &addressInfo) != 0)
    {
        throw std::runtime_error("getaddrinfo() failed with error code: " + std::to_string(WSAGetLastError()));
    }

    g_SocketHandle = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
    if (g_SocketHandle == INVALID_SOCKET)
    {
        throw std::runtime_error("socket() failed with error code: " + std::to_string(WSAGetLastError()));
    }

    if (connect(g_SocketHandle, addressInfo->ai_addr, static_cast<int>(addressInfo->ai_addrlen)) == SOCKET_ERROR)
    {
        throw std::runtime_error("connect() failed with error code: " + std::to_string(WSAGetLastError()));
    }
    freeaddrinfo(addressInfo);
    std::cout << "Socket connected.\n";
}

Socket::~Socket()
{
    if (g_SocketHandle != INVALID_SOCKET)
    {
        closesocket(g_SocketHandle);
    }
    WSACleanup();
}

void Socket::SendData(const std::string& data) const
{
    if (send(g_SocketHandle, data.c_str(), static_cast<int>(data.length()), 0) == SOCKET_ERROR)
    {
        throw std::runtime_error("send() failed with error code: " + std::to_string(WSAGetLastError()));
    }
}

std::unique_ptr<Socket> Socket::CreateConnection(const char* address, int port)
{
    return std::make_unique<Socket>(address, port);
}
#else
#error "Platform not supported."
#endif