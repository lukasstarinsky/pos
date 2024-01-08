#include <Ignis.hpp>
#include <winsock2.h>
#include <stdexcept>
#include "UDPSocket.hpp"

UDPSocket::UDPSocket(const char* address, int port, UDPSocketType type)
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

    auto mode = static_cast<u_long>(type);
    if (ioctlsocket(m_Socket, FIONBIO, &mode) == SOCKET_ERROR)
    {
        throw std::runtime_error("ioctlsocket() failed with error code: " + std::to_string(WSAGetLastError()));
    }

    m_ServerAddr.sin_family = AF_INET;
    m_ServerAddr.sin_port = htons(port);
    m_ServerAddr.sin_addr.s_addr = inet_addr(address);
    IGNIS_INFO("Initialized {} socket to: {}:{}", type == UDPSocketType::BLOCKING ? "blocking": "non-blocking", address, port);
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
//        IGNIS_ERROR("sendto() failed with error code: {}", WSAGetLastError());
    }
}

std::string UDPSocket::ReadData()
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    if (recvfrom(m_Socket, buffer, sizeof(buffer), 0, (sockaddr*)&m_ServerAddr, &m_ServerAddrLen) == SOCKET_ERROR)
    {
//        IGNIS_ERROR("recvfrom() failed with error code: {}", WSAGetLastError());
    }

    std::string output = buffer;
    std::erase(output, '\n');
    return output;
}