#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include "Socket/UDPSocket.hpp"

#define PORT 12694

struct ThreadData
{
    UDPSocketClient& Player;
    std::queue<std::string> MessageQueue;
    bool Stop = false;
};

struct Player
{
    sockaddr_in SockAddr;
};

int main()
{
    UDPSocketServer server(PORT);
    UDPSocketClient client;

    std::vector<Player> players;
    bool gameStarted = false;

    while (true)
    {
        if (players.size() < 2)
        {
            std::cout << "Waiting for players to join (" << players.size() << "/2)...\n";

            while (players.size() < 2)
            {
                std::string data;
                if (client.TryReadData(server(), data))
                {
                    if (data == "join")
                    {
                        players.emplace_back(client.m_SockAddr);
                        client.SendData(server(), "ID-" + std::to_string(players.size() - 1), players[players.size() - 1].SockAddr);
                        std::cout << "Player connected (" << players.size() << "/2).\n";
                    }
                }
            }
            std::cout << "All players connected. Game starting...\n";
        }

        if (!gameStarted)
        {
            for (auto& player: players)
            {
                client.SendData(server(), "START", player.SockAddr);
            }

            gameStarted = true;
        }

        if (gameStarted)
        {
            std::string data;
            if (client.TryReadData(server(), data))
            {
                if (data.starts_with("0Z-"))
                {
                    client.SendData(server(), data, players[1].SockAddr);
                }
                else if (data.starts_with("1Z-"))
                {
                    client.SendData(server(), data, players[0].SockAddr);
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}