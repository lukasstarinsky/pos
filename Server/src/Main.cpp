#include <iostream>
#include <vector>
#include <thread>
#include "Socket/Socket.hpp"

#define PORT 12694

int main()
{
    auto server = SocketServer::Create(/*port*/PORT);
    std::cout << "Server running on port: " << PORT << std::endl;


    SocketConnection* player1 = nullptr;
    SocketConnection* player2 = nullptr;
    while (true)
    {
        if (!player1)
        {
            std::cout << "Waiting for players to connect...\n";
            player1 = server->AcceptConnection();
            std::cout << "Player connected.\n";
        }
        if (!player2)
        {
            std::cout << "Waiting for players to connect...\n";
            player2 = server->AcceptConnection();
            std::cout << "Player connected.\n";
        }

        std::string dataPlayer1;
        if (!player1->TryReadData(dataPlayer1, ';'))
        {
            std::cerr << "Player1 disconnected\n";
            delete player1;
            player1 = nullptr;
            continue;
        }
        else
        {
            player2->TrySendData(dataPlayer1);
        }

        std::string dataPlayer2;
        if (!player2->TryReadData(dataPlayer2, ';'))
        {
            std::cerr << "Player2 disconnected\n";
            delete player2;
            player2 = nullptr;
            continue;
        }
        else
        {
            player1->TrySendData(dataPlayer2);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}