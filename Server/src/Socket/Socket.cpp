#include <cstring>
#include "Socket.hpp"

#pragma region "Linux implementation"
namespace
{
    socklen_t g_AddrLength;
    sockaddr_in g_Addr;
}

SocketServer::SocketServer(int port)
{
    m_Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_Socket < 0)
    {
        throw std::runtime_error("socket() failed.");
    }

    g_Addr = {};
    g_Addr.sin_family = AF_INET;
    g_Addr.sin_addr.s_addr = INADDR_ANY;
    g_Addr.sin_port = htons(port);
    if (bind(m_Socket, (sockaddr*)&g_Addr, sizeof(g_Addr)) < 0)
    {
        throw std::runtime_error("bind() failed.");
    }

    if (listen(m_Socket, SOMAXCONN) < 0)
    {
        throw std::runtime_error("listen() failed.");
    }

    int opt;
    if (setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        throw std::runtime_error("setsockopt() failed.");
    }

    g_AddrLength = sizeof(g_Addr);
}

SocketServer::~SocketServer()
{
    close(m_Socket);
}

std::unique_ptr<SocketServer> SocketServer::Create(int port)
{
    return std::make_unique<SocketServer>(port);
}

SocketConnection* SocketServer::AcceptConnection()
{
    int connection = accept(m_Socket, (sockaddr*)&g_Addr, &g_AddrLength);
    if (connection < 0)
    {
        throw std::runtime_error("accept() failed.");
    }
    return new SocketConnection(connection);
}

SocketConnection::SocketConnection(int connection)
    : m_Connection(connection)
{

}

SocketConnection::~SocketConnection()
{
    close(m_Connection);
}

int SocketConnection::operator()() const
{
    return m_Connection;
}

bool SocketConnection::TryReadData(std::string& output, char delimiter) const
{
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    int bytesReceived = recv(m_Connection, buffer, sizeof(buffer), 0);

    if (bytesReceived <= 0)
    {
        return false;
    }

    std::string bufferString = buffer;
    output = bufferString.substr(0, bufferString.find(delimiter));
    return true;
}

bool SocketConnection::TrySendData(const std::string& data) const
{
    if (send(m_Connection, data.c_str(), data.size(), 0) < 0)
    {
        return false;
    }
    return true;
}
#pragma endregion