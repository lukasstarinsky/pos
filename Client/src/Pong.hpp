#pragma once

#include <Ignis.hpp>
#include <thread>
#include <condition_variable>
#include "MessageQueue.hpp"
#include "Cube.hpp"
#include "Socket/UDPSocket.hpp"

class Pong : public Application
{
public:
    Pong(const ApplicationProperties& appProperties);
    ~Pong();

    void OnUpdate(f64 deltaTimeSeconds) override;
    void OnRender() override;
    void OnImGUIRender() override;
    void OnResize() override;

    bool IsBallOutOfBounds() const;
private:
    void SocketReader();
    void SocketSender();
    void UpdateBall(f64 deltaTimeSeconds);

    bool OnKeyEvent(Event& e);
    f64 GetElapsedSinceLastUpdate();
private:
    std::vector<std::unique_ptr<Cube>> m_Map;
    u8 m_TopIndex, m_BottomIndex, m_RightIndex, m_LeftIndex, m_FloorIndex, m_Player;
    std::unique_ptr<Cube> m_Ball;
    std::unique_ptr<Cube> m_Players[2];
    u8 m_Player1Score = 0, m_Player2Score = 0;
    std::unique_ptr<Camera> m_Camera;

    std::unique_ptr<UDPSocket> m_Socket;
    std::thread m_ReadThread;
    std::thread m_SendThread;
    std::mutex m_Mutex;
    MessageQueue m_SendQueue;

    f64 m_LastDataSent = 0.0f;

    bool m_IsGameStarted = false;
    bool m_ShouldStop = false;
};