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

            UDPSocketClient player;
            std::string data = player.WaitForData(server());

            if (data != "join")
            {
                continue;
            }

            player.SendData(server(), "TESTING");
            // Todo: Join user
        }

        std::this_thread::sleep_for(std::chrono::microseconds((1000 / 64) * 1000)); // 64 - tick ? :)
    }

    return 0;
}