#pragma once

#include <Blaze.hpp>
#include "Cube.hpp"

class Pong : public Application
{
public:
    Pong(const ApplicationProperties& appProperties);
    ~Pong();

    void OnUpdate(f64 deltaTimeSeconds) override;
    void OnRender() override;
    void OnImGUIRender() override;
    void OnResize() override;
private:
    void UpdateBall(f64 deltaTimeSeconds);
    std::vector<std::unique_ptr<Cube>> m_Map;
    std::unique_ptr<Cube> m_Ball;
    std::unique_ptr<Cube> m_Floor;
    std::unique_ptr<Cube> m_Player;
    std::unique_ptr<Cube> m_Opponent;
    Camera m_Camera;
};