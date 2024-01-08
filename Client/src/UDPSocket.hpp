#pragma once

enum class UDPSocketType : u32
{
    BLOCKING = 0,
    NON_BLOCKING
};

class UDPSocket
{
public:
    UDPSocket(const char* address, int port, UDPSocketType type);
    ~UDPSocket();

    std::string ReadData();
    void SendData(const std::string& data);
private:
    SOCKET m_Socket;
    sockaddr_in m_ServerAddr = {};
    int m_ServerAddrLen = sizeof(m_ServerAddr);
};