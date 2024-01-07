#include <Ignis.hpp>
#include <winsock2.h>
#include <stdexcept>
#include "UDPSocket.hpp"

UDPSocket::UDPSocket(const char* address, int port)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        throw std::runtime_error("WSAStartup() failed with error code: " + std::to_string(WSAGetLastError()));
    }

    m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_Socket == INVALID_SOCKET)
    {
        throw std::runtime_error("socket() failed with error code: " + std::to_string(WSAGetLastError()));
    }

    m_ServerAddr.sin_family = AF_INET;
    m_ServerAddr.sin_port = htons(port);
    m_ServerAddr.sin_addr.s_addr = inet_addr(address);
    IGNIS_INFO("Connected to: {}:{}", address, port);
}

UDPSocket::~UDPSocket()
{
    closesocket(m_Socket);
    WSACleanup();
}

void UDPSocket::SendData(const std::string& data)
{
    if (sendto(m_Socket, data.c_str(), data.length(), 0, (sockaddr*)&m_ServerAddr, sizeof(m_ServerAddr)) == SOCKET_ERROR)
    {
        IGNIS_ERROR("sendto() failed with error code: {}", WSAGetLastError());
    }
}