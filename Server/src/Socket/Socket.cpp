#include "Socket.hpp"

#pragma region "Linux implementation"
#if defined(__linux__)
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

    if (listen(m_Socket, 2) < 0)
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

SocketConnection SocketServer::AcceptConnection()
{
    int connection = accept(m_Socket, (sockaddr*)&g_Addr, &g_AddrLength);
    if (connection < 0)
    {
        throw std::runtime_error("accept() failed.");
    }
    return connection;
}

SocketConnection::SocketConnection(int connection)
    : m_Connection(connection)
{

}

SocketConnection::~SocketConnection()
{
    close(m_Connection);
}

bool SocketConnection::ReadData(std::string* outData) const
{
    char buffer[256];
    if (read(m_Connection, &buffer, sizeof(buffer)) <= 0)
    {
        return false;
    }

    if (outData)
    {
        *outData = std::string(buffer);
    }
    return true;
}

std::unique_ptr<SocketServer> SocketServer::Create(int port)
{
    return std::make_unique<SocketServer>(port);
}
#endif
#pragma endregion