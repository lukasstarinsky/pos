#pragma once

#include <vector>
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

    int GetPlayerCount() const;
    GameState GetGameState() const;
    void ConnectPlayer();
    void StartGame();
    bool CheckForDisconnect(int timeoutSeconds);
    void UpdateState();
private:
    std::vector<Player> m_Players;
    UDPSocketServer* m_SocketServer;
    UDPSocketClient* m_SocketClient;
    GameState m_GameState = GameState::Connecting;
    std::chrono::time_point<std::chrono::steady_clock> m_LastUpdate;
};