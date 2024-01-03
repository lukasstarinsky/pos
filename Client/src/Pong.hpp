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

    bool OnAppEvent(Event& e);
private:
    Cube m_Cube;
    Camera m_Camera;
};