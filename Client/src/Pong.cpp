#include "Pong.hpp"

Pong::Pong(const ApplicationProperties& appProperties)
    : Application(appProperties),
      m_Camera(CameraProjection::Perspective, {0.0f, 250.0f, 0.0f}, appProperties.WindowWidth, appProperties.WindowHeight, 45.0f, 0.1f, 100.0f)
{

    // Floor
    m_Floor = std::make_unique<Cube>();
    m_Floor->SetScale({500, 1, 500});
    m_Floor->SetPosition({0, 0, 0});

    // Borders/Walls
    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetColor({0.0f, 1.0f, 0.0f});
    m_Map[m_Map.size() - 1]->SetScale({235, 10, 10});
    m_Map[m_Map.size() - 1]->SetPosition({0, 0, -90});

    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetColor({0.0f, 1.0f, 0.0f});
    m_Map[m_Map.size() - 1]->SetScale({235, 10, 10});
    m_Map[m_Map.size() - 1]->SetPosition({0, 0, 90});

    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetColor({0.0f, 1.0f, 0.0f});
    m_Map[m_Map.size() - 1]->SetScale({10, 10, 190});
    m_Map[m_Map.size() - 1]->SetPosition({115, 0, 0});

    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetColor({0.0f, 1.0f, 0.0f});
    m_Map[m_Map.size() - 1]->SetScale({10, 10, 190});
    m_Map[m_Map.size() - 1]->SetPosition({-115, 0, 0});

    //Lines
    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetColor({1.0f, 1.0f, 1.0f});
    m_Map[m_Map.size() - 1]->SetScale({3, 1.25, 30});
    m_Map[m_Map.size() - 1]->SetPosition({0, 0, -63});

    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetColor({1.0f, 1.0f, 1.0f});
    m_Map[m_Map.size() - 1]->SetScale({3, 1.25, 30});
    m_Map[m_Map.size() - 1]->SetPosition({0, 0, -21});

    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetColor({1.0f, 1.0f, 1.0f});
    m_Map[m_Map.size() - 1]->SetScale({3, 1.25, 30});
    m_Map[m_Map.size() - 1]->SetPosition({0, 0, 21});

    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetColor({1.0f, 1.0f, 1.0f});
    m_Map[m_Map.size() - 1]->SetScale({3, 1.25, 30});
    m_Map[m_Map.size() - 1]->SetPosition({0, 0, 63});

    // Camera, ball, players
    m_Camera.Rotate(0.0f, -89.9f);

    m_Ball = std::make_unique<Cube>(true);
    m_Ball->SetColor(glm::vec3(1.0f));
    m_Ball->SetScale({10.0f, 10.0f, 10.0f});
    m_Ball->SetPosition({0.0f, 5.0f, 0.0f});

    //Player 1
    m_Player = std::make_unique<Cube>();
    m_Player->SetColor({0.0f, 0.0f, 1.0f});
    m_Player->SetScale({10, 10, 40});
    m_Player->SetPosition({-100, 0, 0});

    //Player 2
    m_Opponent = std::make_unique<Cube>();
    m_Opponent->SetColor({0.0f, 0.0f, 1.0f});
    m_Opponent->SetScale({10, 10, 40});
    m_Opponent->SetPosition({100, 0, 0});
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

    m_Ball->Draw(m_Camera);
    m_Floor->DrawLit(m_Camera, m_Ball);
    m_Player->DrawLit(m_Camera, m_Ball);
    m_Opponent->DrawLit(m_Camera, m_Ball);
    for (const auto &item: m_Map) {
        item->DrawLit(m_Camera, m_Ball);
    }
}

static void GenerateCode(const std::unique_ptr<Cube>& cube)
{
    BLAZE_INFO("\nm_Map.emplace_back(std::make_unique<Cube>());\n"
               "m_Map[m_Map.size() - 1]->SetColor({{0.0f, 1.0f, 0.0f}});\n"
               "m_Map[m_Map.size() - 1]->SetScale({{{}, {}, {}}});\n"
               "m_Map[m_Map.size() - 1]->SetPosition({{{}, {}, {}}});\n",
               cube->Scale.x, cube->Scale.y, cube->Scale.z,
               cube->Position.x, cube->Position.y, cube->Position.z);
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