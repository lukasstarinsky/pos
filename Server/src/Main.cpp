#include <iostream>
#include <vector>
#include <thread>
#include "Socket/UDPSocket.hpp"

#define PORT 12694

int main()
{
    UDPSocketServer server(PORT);

    std::vector<UDPSocketClient> players;
    while (true)
    {
        if (players.size() < 1)
        {
            // Todo: thread for each player?
            std::cout << "Waiting for players to join(" << players.size() << "/2)...\n";

            while (players.size() < 1)
            {
                UDPSocketClient player;
                std::string data;
                if (player.TryReadData(server(), data))
                {
                    if (data == "join")
                    {
                        std::cout << "Player connected(" << players.size() << "/2)\n";
                        players.emplace_back(player);
                        player.SendData(server(), "TESTING");
                    }
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::microseconds((1000 / 64) * 1000)); // 64 - tick ? :)
    }

    return 0;
}