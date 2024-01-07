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

    int operator()() const;

    bool TryReadData(std::string& output, char delimiter = ';') const;
    bool TrySendData(const std::string& data) const;
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