#include "Pong.hpp"

Pong::Pong(const ApplicationProperties& appProperties)
    : Application(appProperties),
      m_Camera(CameraProjection::Perspective, {0.0f, 275.0f, 0.0f}, appProperties.WindowWidth, appProperties.WindowHeight, 45.0f, 0.1f, 100.0f)
{
    // Floor
    m_Floor = std::make_unique<Cube>();
    m_Floor->SetScale({500, 1, 500});
    m_Floor->SetPosition({0, 0, 0});

    // Top
    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetScale({280, 10, 10});
    m_Map[m_Map.size() - 1]->SetPosition({0, 0, -90});

    // Bottom
    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetScale({280, 10, 10});
    m_Map[m_Map.size() - 1]->SetPosition({0, 0, 90});

    // Right
    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetScale({10, 10, 190});
    m_Map[m_Map.size() - 1]->SetPosition({145, 0, 0});

    // Left
    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetScale({10, 10, 190});
    m_Map[m_Map.size() - 1]->SetPosition({-145, 0, 0});

    //Lines
    m_Map.emplace_back(std::make_unique<Cube>(true));
    m_Map[m_Map.size() - 1]->SetScale({3, 1.25, 30});
    m_Map[m_Map.size() - 1]->SetPosition({0, 0, -63});

    m_Map.emplace_back(std::make_unique<Cube>(true));
    m_Map[m_Map.size() - 1]->SetScale({3, 1.25, 30});
    m_Map[m_Map.size() - 1]->SetPosition({0, 0, -21});

    m_Map.emplace_back(std::make_unique<Cube>(true));
    m_Map[m_Map.size() - 1]->SetScale({3, 1.25, 30});
    m_Map[m_Map.size() - 1]->SetPosition({0, 0, 21});

    m_Map.emplace_back(std::make_unique<Cube>(true));
    m_Map[m_Map.size() - 1]->SetScale({3, 1.25, 30});
    m_Map[m_Map.size() - 1]->SetPosition({0, 0, 63});

    // Camera, ball, players
    m_Camera.Rotate(0.0f, -89.9f);

    m_Ball = std::make_unique<Cube>(true);
    m_Ball->SetScale({10.0f, 10.0f, 10.0f});
    m_Ball->SetColor({1.0f, 0.0f, 0.0f});
    m_Ball->SetPosition({0.0f, 5.0f, 0.0f});

    //Player 1
    m_Player = std::make_unique<Cube>(true);
    m_Player->SetScale({10, 10, 40});
    m_Player->SetPosition({-125, 0, 0});

    //Player 2
    m_Opponent = std::make_unique<Cube>(true);
    m_Opponent->SetScale({10, 10, 40});
    m_Opponent->SetPosition({125, 0, 0});
}

Pong::~Pong()
{
}

void Pong::OnUpdate(f64 deltaTimeSeconds)
{
    if (Input::IsKeyDown(Key::UpArrow) || Input::IsKeyDown(Key::W))
    {
        m_Player->Position.z -= 80.0f * (f32)deltaTimeSeconds;

        f32 wallPos = m_Map[0]->Position.z + m_Map[0]->Scale.z;
        if (m_Player->Position.z - m_Player->Scale.z / 2 < wallPos)
        {
            m_Player->Position.z = wallPos + m_Player->Scale.z / 2;
        }
    }
    else if (Input::IsKeyDown(Key::DownArrow) || Input::IsKeyDown(Key::S))
    {
        m_Player->Position.z += 80.0f * (f32)deltaTimeSeconds;

        f32 wallPos = m_Map[1]->Position.z - m_Map[1]->Scale.z;
        if (m_Player->Position.z + m_Player->Scale.z / 2 > wallPos)
        {
            m_Player->Position.z = wallPos - m_Player->Scale.z / 2;
        }
    }
}

void Pong::OnRender()
{
    Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    Renderer::ClearBuffer();

    m_Ball->Draw(m_Camera);
    m_Floor->DrawLit(m_Camera, m_Ball);
    m_Player->DrawLit(m_Camera, m_Ball);
    m_Opponent->DrawLit(m_Camera, m_Ball);
    for (const auto &item: m_Map) {
        item->DrawLit(m_Camera, m_Ball);
    }
}

void Pong::OnImGUIRender()
{
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::Begin("Debug");
    {
        if (ImGui::TreeNode("Light"))
        {
            ImGui::CollapsingHeader("Light");
            ImGui::DragFloat3("Position", (float*)&m_Ball->Position, 0.2f);
            ImGui::ColorPicker3("Color", (f32*)&m_Ball->Color);

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void Pong::OnResize()
{
    m_Camera.SetViewport(m_Properties.WindowWidth, m_Properties.WindowHeight);
}