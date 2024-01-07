#include <iostream>
#include <vector>
#include <thread>
#include "Socket/Socket.hpp"

#define PORT 12694

int main()
{
    auto server = SocketServer::Create(/*port*/PORT);
    std::cout << "Server running on port: " << PORT << std::endl;

    SocketConnection client = server->AcceptConnection();
    while (true)
    {
        std::string data;
        if (!client.TryReadData(data, ';'))
        {
            std::cerr << "Client disconnected\n";
            break;
        }
        else
        {
            std::cout << "Client sent: " << data << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}