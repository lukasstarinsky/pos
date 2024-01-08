#include <chrono>
#include <iostream>
#include "Game.hpp"

Game::Game(UDPSocketServer* socketServer, UDPSocketClient* socketClient)
    : m_SocketServer(socketServer), m_SocketClient(socketClient)
{

}

void Game::ConnectPlayer()
{
    std::string data;
    if (m_SocketClient->TryReadData(m_SocketServer->GetSocket(), data))
    {
        if (data == "join")
        {
            Player player = {};
            player.LastUpdate = std::chrono::steady_clock::now();
            player.SockAddr = m_SocketClient->GetSockAddr();
            player.ID = static_cast<int>(m_Players.size());
            m_Players.emplace_back(player);
            std::cout << "Player connected (" << m_Players.size() << "/2).\n";

            m_SocketClient->SendData(m_SocketServer->GetSocket(), "ID-" + std::to_string(player.ID), player.SockAddr);
        }
    }
}

void Game::StartGame()
{
    m_GameState = GameState::Started;

    for (auto& player: m_Players)
    {
        player.LastUpdate = std::chrono::steady_clock::now();
        m_SocketClient->SendData(m_SocketServer->GetSocket(), "START", player.SockAddr);
    }
}

bool Game::CheckForDisconnect(int timeoutSeconds)
{
    unsigned erased = std::erase_if(m_Players, [&](const Player& player) {
        const auto now = std::chrono::steady_clock::now();
        const std::chrono::duration<double> diff = now - player.LastUpdate;

        if (diff.count() >= timeoutSeconds)
        {
            m_SocketClient->SendData(m_SocketServer->GetSocket(), "QUIT", player.SockAddr);
        }

        return diff.count() >= timeoutSeconds;
    });

    if (erased > 0)
    {
        m_GameState = GameState::Connecting;
        std::cout << erased << " player(s) disconnected...\n";
    }

    return erased > 0;
}

void Game::UpdatePlayerPositions()
{
    std::string data;

    if (m_SocketClient->TryReadData(m_SocketServer->GetSocket(), data))
    {
        if (data.starts_with("0Z-"))
        {
            m_Players[0].LastUpdate = std::chrono::steady_clock::now();
            m_SocketClient->SendData(m_SocketServer->GetSocket(), data, m_Players[1].SockAddr);
        }
        else if (data.starts_with("1Z-"))
        {
            m_Players[1].LastUpdate = std::chrono::steady_clock::now();
            m_SocketClient->SendData(m_SocketServer->GetSocket(), data, m_Players[0].SockAddr);
        }
    }
}

int Game::GetPlayerCount() const
{
    return m_Players.size();
}

GameState Game::GetGameState() const
{
    return m_GameState;
}