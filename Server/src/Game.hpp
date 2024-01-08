#pragma once

#include <vector>
#include <mutex>
#include "Socket/UDPSocket.hpp"

enum class GameState
{
    Connecting = 0,
    Started
};

struct Player
{
    int ID;
    sockaddr_in SockAddr;
    std::chrono::time_point<std::chrono::steady_clock> LastUpdate;
};

class Game
{
public:
    Game(UDPSocketServer* socketServer, UDPSocketClient* socketClient);
    ~Game();

    int GetPlayerCount();
    GameState GetGameState();
    void ConnectPlayer();
    void DisconnectPlayer(int ID);
    void StartGame();
    bool CheckForDisconnect(int timeoutSeconds);
    void UpdateState();
private:
    std::array<Player*, 2> m_Players = {nullptr, nullptr };
    UDPSocketServer* m_SocketServer;
    UDPSocketClient* m_SocketClient;
    GameState m_GameState = GameState::Connecting;
    std::chrono::time_point<std::chrono::steady_clock> m_LastUpdate;
};