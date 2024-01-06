#include <iostream>
#include <vector>
#include "Socket/Socket.hpp"

#define PORT 12694

int main()
{
    auto server = SocketServer::Create(/*port*/PORT);
    std::cout << "Server running on port: " << PORT << std::endl;

    std::vector<SocketConnection> players;
    while (true)
    {
        if (players.empty() || players.size() == 1)
        {
            std::cout << "Waiting for players to connect: " << players.size() << "/2\n";
            players.emplace_back(server->AcceptConnection());
            std::cout << "Player connected\n";
            continue;
        }

        unsigned int disconnected = std::erase_if(players, [](const SocketConnection& player) {
           return !player.ReadData(nullptr);
        });
        if (disconnected > 0)
        {
            std::cout << "Player disconnected.\n";
            break;
        }
//
//        std::string player1Pos, player2Pos;
//        players[0].ReadData(&player1Pos);
//        players[1].ReadData(&player2Pos);
    }

    return 0;
}