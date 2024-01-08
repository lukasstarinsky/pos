#include <iostream>
#include <thread>
#include "Game.hpp"
#include "Socket/UDPSocket.hpp"

#define PORT 12694
#define PLAYER_TIMEOUT_SECONDS 10

std::mutex g_Mutex;

void ConnectionHandler(const std::unique_ptr<Game>& game)
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(g_Mutex);
        if (game->GetGameState() == GameState::Terminate)
        {
            break;
        }
        else if (game->GetGameState() == GameState::Connecting && game->GetPlayerCount() != 2)
        {
            game->ConnectPlayer();
        }
        else if (game->GetGameState() != GameState::Started && game->GetPlayerCount() == 2)
        {
            game->StartGame();
        }
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main()
{
    auto server = std::make_unique<UDPSocketServer>(PORT);
    auto client = std::make_unique<UDPSocketClient>();
    auto game = std::make_unique<Game>(server.get(), client.get());

    std::thread connectionHandler(ConnectionHandler, std::ref(game));
    while (true)
    {
        std::unique_lock<std::mutex> lock(g_Mutex);
        if (game->GetGameState() == GameState::Terminate)
        {
            break;
        }
        game->UpdateState();
        game->CheckForDisconnect(PLAYER_TIMEOUT_SECONDS);
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    connectionHandler.join();

    return 0;
}