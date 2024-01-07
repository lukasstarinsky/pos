#pragma once

#include <Ignis.hpp>
#include <thread>
#include <condition_variable>
#include "Cube.hpp"
#include "Socket/Socket.hpp"

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

    void SocketCommunication() const;
private:
    void UpdateBall(f64 deltaTimeSeconds);
    std::vector<std::unique_ptr<Cube>> m_Map;

    u8 m_TopIndex, m_BottomIndex, m_RightIndex, m_LeftIndex, m_FloorIndex;
    std::unique_ptr<Cube> m_Ball;

    std::condition_variable m_CanUpdate;
    std::mutex m_Mutex;
    std::unique_ptr<Cube> m_Player;
    std::unique_ptr<Cube> m_Opponent;

    std::unique_ptr<Camera> m_Camera;
    std::unique_ptr<Socket> m_Socket;

    std::thread m_CommunicationThread;
};