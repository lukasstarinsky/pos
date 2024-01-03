#include <iostream>
#include <vector>
#include "Socket/Socket.hpp"

int main()
{
    auto server = SocketServer::Create(/*port*/16912);

    std::vector<SocketConnection> players;
    while (true)
    {
        if (players.empty() || players.size() == 1)
        {
            std::cout << "Waiting for players to connect\n";
            players.emplace_back(server->AcceptConnection());
        }

        unsigned int disconnected = std::erase_if(players, [](const SocketConnection& player) {
           return !player.ReadData(nullptr);
        });
        if (disconnected > 0)
        {
            std::cout << "Player disconnected.\n";
            continue;
        }

        std::string player1Pos, player2Pos;
        players[0].ReadData(&player1Pos);
        players[1].ReadData(&player2Pos);
    }

    return 0;
}