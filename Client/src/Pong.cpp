#include "Pong.hpp"

Pong::Pong(const ApplicationProperties& appProperties)
    : Application(appProperties),
      m_Camera(CameraProjection::Perspective, {0.0f, 0.0f, 0.0f}, appProperties.WindowWidth, appProperties.WindowHeight, 45.0f, 0.1f, 100.0f)
{
    m_LightSource = std::make_unique<Cube>(true);
    m_LightSource->SetColor(glm::vec3(1.0f));

    //Borders/Walls

    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetColor({0.0f, 1.0f, 0.0f});
    m_Map[m_Map.size() - 1]->SetScale({228, 10, 3});
    m_Map[m_Map.size() - 1]->SetPosition({0, 0, -90});

    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetColor({0.0f, 1.0f, 0.0f});
    m_Map[m_Map.size() - 1]->SetScale({228, 10, 3});
    m_Map[m_Map.size() - 1]->SetPosition({0, 0, 90});

    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetColor({0.0f, 1.0f, 0.0f});
    m_Map[m_Map.size() - 1]->SetScale({3, 10, 183});
    m_Map[m_Map.size() - 1]->SetPosition({115, 0, 0});

    m_Map.emplace_back(std::make_unique<Cube>());
    m_Map[m_Map.size() - 1]->SetColor({0.0f, 1.0f, 0.0f});
    m_Map[m_Map.size() - 1]->SetScale({3, 10, 183});
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

    //Player 1
    m_Player = std::make_unique<Cube>();
    m_Player->SetColor({0.0f, 0.0f, 1.0f});
    m_Player->SetScale({3, 10, 30});
    m_Player->SetPosition({-100, 0, 0});

    //Player 2
    m_Opponent = std::make_unique<Cube>();
    m_Opponent->SetColor({0.0f, 0.0f, 1.0f});
    m_Opponent->SetScale({3, 10, 30});
    m_Opponent->SetPosition({100, 0, 0});

    m_DebugCube = std::make_unique<Cube>();
    m_Floor = std::make_unique<Cube>();
    m_Floor->SetScale({500, 1, 500});
    m_Floor->SetPosition({0, 0, 0});
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

    m_LightSource->Draw(m_Camera);
    m_Floor->DrawLit(m_Camera, m_LightSource);
    m_DebugCube->DrawLit(m_Camera, m_LightSource);
    m_Player->DrawLit(m_Camera, m_LightSource);
    m_Opponent->DrawLit(m_Camera, m_LightSource);
    for (const auto &item: m_Map) {
        item->DrawLit(m_Camera, m_LightSource);
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
        if (ImGui::TreeNode("Cubes"))
        {
            ImGui::CollapsingHeader("Cube 1");
            ImGui::DragFloat3("Position", (float*)&m_DebugCube->Position, 0.2f);
            ImGui::DragFloat3("Rotation", (float*)&m_DebugCube->Rotation, 0.2f);
            ImGui::DragFloat3("Scale", (float*)&m_DebugCube->Scale, 0.2f);
            ImGui::ColorPicker3("Color", (f32*)&m_DebugCube->Color);
            if (ImGui::Button("Generate code"))
            {
                GenerateCode(m_DebugCube);
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Light"))
        {
            ImGui::CollapsingHeader("Light");
            ImGui::DragFloat3("Position", (float*)&m_LightSource->Position, 0.2f);
            ImGui::ColorPicker3("Color", (f32*)&m_LightSource->Color);

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void Pong::OnResize()
{
    m_Camera.SetViewport(m_Properties.WindowWidth, m_Properties.WindowHeight);
}