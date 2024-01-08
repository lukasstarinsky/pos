#include <iostream>
#include <thread>
#include "Game.hpp"
#include "Socket/UDPSocket.hpp"

#define PORT 12694
#define PLAYER_TIMEOUT_SECONDS 10

int main()
{
    auto server = std::make_unique<UDPSocketServer>(PORT);
    auto client = std::make_unique<UDPSocketClient>();
    auto game = std::make_unique<Game>(server.get(), client.get());

    while (true)
    {
        // Connect players
        if (game->GetGameState() == GameState::Connecting && game->GetPlayerCount() != 2)
        {
            std::cout << "Waiting for players to connect (" << game->GetPlayerCount() << "/2)...\n";
            while (game->GetPlayerCount() != 2)
            {
                game->ConnectPlayer();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            std::cout << "All players connected, starting the game...\n";
            game->StartGame();
        }
        // Update the game state
        else if (game->GetGameState() == GameState::Started)
        {
            if (game->CheckForDisconnect(PLAYER_TIMEOUT_SECONDS))
            {
                continue;
            }
            game->UpdateState();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}