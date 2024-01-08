#include <chrono>
#include <iostream>
#include "Game.hpp"

static bool CompareAdresses(const sockaddr_in& addr1, const sockaddr_in& addr2)
{
    return ((addr1.sin_addr.s_addr == addr2.sin_addr.s_addr) && (addr1.sin_port == addr2.sin_port));
}

Game::Game(UDPSocketServer* socketServer, UDPSocketClient* socketClient)
    : m_SocketServer(socketServer), m_SocketClient(socketClient)
{

}

Game::~Game()
{
    for (Player* player: m_Players)
    {
        if (player)
        {
            delete player;
        }
    }
}

void Game::ConnectPlayer()
{
    std::string data;
    if (m_SocketClient->TryReadData(m_SocketServer->GetSocket(), data, true))
    {
        if (data == "JOIN")
        {
            for (Player* player: m_Players)
            {
                if (player && CompareAdresses(player->SockAddr, m_SocketClient->GetSockAddr()))
                {
                    std::string discard;
                    m_SocketClient->TryReadData(m_SocketServer->GetSocket(), discard);

                    return;
                }
            }

            Player* player = new Player();
            player->LastUpdate = std::chrono::steady_clock::now();
            player->SockAddr = m_SocketClient->GetSockAddr();
            player->ID = !m_Players[0] ? 0 : !m_Players[1] ? 1 : -1;
            if (player->ID == -1)
            {
                return;
            }

            m_Players[player->ID] = player;

            std::cout << "Player connected (" << GetPlayerCount() << "/2).\n";
            m_SocketClient->SendData(m_SocketServer->GetSocket(), "ID-" + std::to_string(player->ID), player->SockAddr);
        }
    }
}

void Game::DisconnectPlayer(int ID)
{
    if (m_Players[ID] == nullptr)
    {
        return;
    }

    delete m_Players[ID];
    m_Players[ID] = nullptr;
    std::cout << "Player #" << ID << " disconnected (" << GetPlayerCount() << "/2)...\n";

    if (m_GameState == GameState::Terminate)
    {
        return;
    }

    for (Player* player: m_Players)
    {
        if (player)
        {
            m_SocketClient->SendData(m_SocketServer->GetSocket(), "PAUSE", player->SockAddr);
        }
    }
}

void Game::StartGame()
{
    m_GameState = GameState::Started;

    for (Player* player: m_Players)
    {
        if (player)
        {
            player->LastUpdate = std::chrono::steady_clock::now();
            m_SocketClient->SendData(m_SocketServer->GetSocket(), "START", player->SockAddr);
        }
    }

    std::cout << "Game started!\n";
    m_LastUpdate = std::chrono::steady_clock::now();
}

bool Game::CheckForDisconnect(int timeoutSeconds)
{
    auto now = std::chrono::steady_clock::now();

    int disconnected = 0;
    for (int i = 0; i < m_Players.size(); ++i)
    {
        Player* player = m_Players[i];

        if (player)
        {
            std::chrono::duration<double> diff = now - player->LastUpdate;
            if (diff.count() >= timeoutSeconds)
            {
                m_SocketClient->SendData(m_SocketServer->GetSocket(), "QUIT", player->SockAddr);
                DisconnectPlayer(i);
                m_GameState = GameState::Connecting;
                ++disconnected;
            }
        }
    }

    return disconnected > 0;
}

void Game::UpdateState()
{
    std::string data;

    if (m_SocketClient->TryReadData(m_SocketServer->GetSocket(), data, true))
    {
        if (data == "JOIN" && m_GameState != GameState::Started)
        {
            return;
        }
        else
        {
            std::string discard;
            m_SocketClient->TryReadData(m_SocketServer->GetSocket(), discard);
        }

        if (data == "0Q" || data == "1Q")
        {
            DisconnectPlayer(data[0] - '0');
            m_GameState = GameState::Connecting;
            return;
        }
        else if (data.starts_with("0Z-"))
        {
            m_Players[0]->LastUpdate = std::chrono::steady_clock::now();
            m_SocketClient->SendData(m_SocketServer->GetSocket(), data, m_Players[1]->SockAddr);
        }
        else if (data.starts_with("1Z-"))
        {
            m_Players[1]->LastUpdate = std::chrono::steady_clock::now();
            m_SocketClient->SendData(m_SocketServer->GetSocket(), data, m_Players[0]->SockAddr);
        }
        else if (data == "TERMINATE")
        {
            for (int i = 0; i < m_Players.size(); ++i)
            {
                if (m_Players[i])
                {
                    m_SocketClient->SendData(m_SocketServer->GetSocket(), "QUIT", m_Players[i]->SockAddr);
                    DisconnectPlayer(i);
                }
            }
            m_GameState = GameState::Terminate;
            std::cout << "Server terminating...\n";
            return;
        }
        else if (data.starts_with("BALL-") || data == "RANDOM" || data == "0+" || data == "1+")
        {
            m_SocketClient->SendData(m_SocketServer->GetSocket(), data, m_Players[1]->SockAddr);
        }
    }
}

int Game::GetPlayerCount()
{
    int playerCount = 0;
    for (const Player* player : m_Players)
    {
        if (player != nullptr)
        {
            playerCount++;
        }
    }

    return playerCount;
}

GameState Game::GetGameState()
{
    return m_GameState;
}