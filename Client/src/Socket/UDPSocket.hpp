#pragma once

class UDPSocket
{
public:
    UDPSocket(const char* address, int port);
    ~UDPSocket();

    void SendData(const std::string& data);
private:
    SOCKET m_Socket;
    sockaddr_in m_ServerAddr = {};
};