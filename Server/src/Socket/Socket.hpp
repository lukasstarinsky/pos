#pragma once

#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <memory>

class SocketConnection
{
public:
    SocketConnection(int connection);
    ~SocketConnection();

    bool ReadData(std::string* outData) const;
private:
    int m_Connection;
};

class SocketServer
{
public:
    SocketServer(int port);
    ~SocketServer();

    SocketConnection AcceptConnection();

    static std::unique_ptr<SocketServer> Create(int port);
private:
    int m_Socket;
};