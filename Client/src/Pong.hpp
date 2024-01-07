#pragma once

#include <Blaze.hpp>
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
private:
    void UpdateBall(f64 deltaTimeSeconds);
    std::vector<std::unique_ptr<Cube>> m_Map;
    std::unique_ptr<Cube> m_Players[2];

    u8 m_Player, m_TopIndex, m_BottomIndex, m_RightIndex, m_LeftIndex, m_FloorIndex;
    std::unique_ptr<Cube> m_Ball;
    std::unique_ptr<Cube> m_Opponent;
    std::unique_ptr<Camera> m_Camera;

    std::unique_ptr<Socket> m_Socket;
};