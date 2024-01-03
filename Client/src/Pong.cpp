#include "Pong.hpp"

Pong::Pong(const ApplicationProperties& appProperties)
    : Application(appProperties),
      m_Camera(CameraProjection::Perspective, {0.0f, 0.0f, 0.0f}, appProperties.WindowWidth, appProperties.WindowHeight, 45.0f, 0.1f, 100.0f),
      m_Cube()
{
    SET_EVENT_LISTENER(EventCategory::ApplicationEvent, Pong::OnAppEvent);
}

Pong::~Pong()
{
}

void Pong::OnUpdate(f64 deltaTimeSeconds)
{
    if (Input::IsKeyDown(Key::W))
    {
        m_Camera.MoveZ(-0.5f);
    }
    else if (Input::IsKeyDown(Key::S))
    {
        m_Camera.MoveZ(0.5f);
    }

    if (Input::IsKeyDown(Key::A))
    {
        m_Camera.MoveX(-0.5f);
    }
    else if (Input::IsKeyDown(Key::D))
    {
        m_Camera.MoveX(0.5f);
    }

    if (Input::IsButtonDown(Button::Left))
    {
        while (const auto mouseDelta = Input::ReadMouseRawDelta())
        {
            m_Camera.Rotate((f32)mouseDelta->x * 0.15f, (f32)-mouseDelta->y * 0.15f);
        }
    }
}

void Pong::OnRender()
{
    Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    Renderer::ClearBuffer();

    m_Cube.Draw(m_Camera);

    GraphicsContext::SwapBuffers();
}

bool Pong::OnAppEvent(Event& e)
{
    const auto& appEvent = e.As<const ApplicationEvent&>();

    if (appEvent.GetType() == ApplicationEventType::Resize)
    {
        m_Camera.SetProjection(CameraProjection::Perspective, appEvent.Width, appEvent.Height, 45.0f, 0.1f, 100.0f);
    }

    return false;
}