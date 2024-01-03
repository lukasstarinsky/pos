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
    std::unique_ptr<Cube> m_LightSource;
    std::unique_ptr<Cube> m_Floor;
    std::unique_ptr<Cube> m_DebugCube;
    Camera m_Camera;
};