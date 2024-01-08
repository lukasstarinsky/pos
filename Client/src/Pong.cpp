#include <random>
#include "Pong.hpp"

Pong::Pong(const ApplicationProperties& appProperties)
    : Application(appProperties)
{
    m_Map.reserve(10);

    // Floor
    m_Map.emplace_back(std::make_unique<Cube>());
    m_FloorIndex = m_Map.size() - 1;
    m_Map[m_FloorIndex]->SetScale({500, 1, 500});
    m_Map[m_FloorIndex]->SetPosition({0, 0, 0});

    // Top
    m_Map.emplace_back(std::make_unique<Cube>());
    m_TopIndex = m_Map.size() - 1;
    m_Map[m_TopIndex]->SetScale({280, 10, 10});
    m_Map[m_TopIndex]->SetPosition({0, 0, -90});

    // Bottom
    m_Map.emplace_back(std::make_unique<Cube>());
    m_BottomIndex = m_Map.size() - 1;
    m_Map[m_BottomIndex]->SetScale({280, 10, 10});
    m_Map[m_BottomIndex]->SetPosition({0, 0, 90});

    // Right
    m_Map.emplace_back(std::make_unique<Cube>());
    m_RightIndex = m_Map.size() - 1;
    m_Map[m_RightIndex]->SetScale({10, 10, 190});
    m_Map[m_RightIndex]->SetPosition({145, 0, 0});

    // Left
    m_Map.emplace_back(std::make_unique<Cube>());
    m_LeftIndex = m_Map.size() - 1;
    m_Map[m_LeftIndex]->SetScale({10, 10, 190});
    m_Map[m_LeftIndex]->SetPosition({-145, 0, 0});

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
    m_Camera = std::make_unique<Camera>(CameraProjection::Perspective, glm::vec3(0.0f, 275.0f, 0.0f), appProperties.WindowWidth, appProperties.WindowHeight, 45.0f, 0.1f, 100.0f);
    m_Camera->Rotate(0.0f, -89.9f);

    m_Ball = std::make_unique<Cube>(true);
    m_Ball->SetScale({3.0f, 3.0f, 3.0f});
    m_Ball->SetRandomColor();
    m_Ball->SetPosition({0.0f, 3.0f, 0.0f});

    //Player 1
    m_Players[0] = std::make_unique<Cube>(true);
    m_Players[0]->SetScale({3, 10, 30});
    m_Players[0]->SetPosition({-125, 0, 0});

    //Player 2
    m_Players[1] = std::make_unique<Cube>(true);
    m_Players[1]->SetScale({3, 10, 30});
    m_Players[1]->SetPosition({125, 0, 0});

    m_Player = 0;

    // Socket
    m_Socket = std::make_unique<UDPSocket>("158.193.128.160", 12694, UDPSocketType::NON_BLOCKING);
    m_Socket->SendData("join");
    m_ReadThread = std::thread(&Pong::SocketReader, this);
    m_SendThread = std::thread(&Pong::SocketSender, this);
}

Pong::~Pong()
{
    m_Mutex.lock();
    m_ShouldStop = true;
    m_Mutex.unlock();
    m_SendThread.join();
    m_ReadThread.join();
}

void Pong::SocketSender()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        if (m_ShouldStop)
        {
            IGNIS_INFO("Shutting down SocketSender thread...");
            break;
        }
        lock.unlock();

        std::queue<std::string> queue;
        m_SendQueue.Swap(queue);

        while (!queue.empty())
        {
            std::string data = queue.front();
            queue.pop();
            m_Socket->SendData(data);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Pong::SocketReader()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        if (m_ShouldStop)
        {
            IGNIS_INFO("Shutting down SocketReader thread...");
            break;
        }
        lock.unlock();

        std::string data = m_Socket->ReadData();
        lock.lock();
        if (data == "START")
        {
            m_IsGameRunning = true;
        }
        else if (data == "QUIT")
        {
//            IGNIS_INFO("Quit message received...");
            m_AppState.IsRunning = false;
        }
        else if (data.starts_with("ID-"))
        {
            m_Player = data[3] - '0';
        }
        else if (m_Player == 1 && data.starts_with("0Z-"))
        {
            m_Players[0]->Position.z = std::stof(data.substr(3));
        }
        else if (m_Player == 0 && data.starts_with("1Z-"))
        {
            m_Players[1]->Position.z = std::stof(data.substr(3));
        }
        lock.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Pong::OnUpdate(f64 deltaTimeSeconds)
{
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        if (!m_IsGameRunning)
        {
            return;
        }
    }

    static f32 playerSpeed = 120.0f;

    if (Input::IsKeyDown(Key::UpArrow) || Input::IsKeyDown(Key::W))
    {
        f32 oldZ = m_Players[m_Player]->Position.z;
        m_Players[m_Player]->Position.z -= playerSpeed * (f32)deltaTimeSeconds;

        if (Cube::CheckCollision(*m_Players[m_Player], *m_Map[m_TopIndex]))
        {
            m_Players[m_Player]->Position.z = oldZ;
        }

        m_SendQueue.Push(std::format("{}Z-{}", m_Player, m_Players[m_Player]->Position.z));
    }
    else if (Input::IsKeyDown(Key::DownArrow) || Input::IsKeyDown(Key::S))
    {
        f32 oldZ = m_Players[m_Player]->Position.z;
        m_Players[m_Player]->Position.z += playerSpeed * (f32)deltaTimeSeconds;

        if (Cube::CheckCollision(*m_Players[m_Player], *m_Map[m_BottomIndex]))
        {
            m_Players[m_Player]->Position.z = oldZ;
        }

        m_SendQueue.Push(std::format("{}Z-{}", m_Player, m_Players[m_Player]->Position.z));
    }

    UpdateBall(deltaTimeSeconds);
}

glm::vec3 GetRandomDirection()
{
    static std::default_random_engine randomEngine(Platform::GetTimeSeconds());
    static std::uniform_int_distribution dist(0, 1);

    f32 xDirection = dist(randomEngine) == 0 ? -1 : 1;
    f32 yDirection = dist(randomEngine) == 0 ? -1 : 1;

    return { xDirection, 0.0f, yDirection };
}

void Pong::UpdateBall(f64 deltaTimeSeconds) {
    static glm::vec3 direction = GetRandomDirection();
    static f32 ballMovementSpeed = 120.0f;

    glm::vec3 originalPosition = m_Ball->Position;
    m_Ball->Position.x += direction.x * ballMovementSpeed * (f32)deltaTimeSeconds;
    m_Ball->Position.z += direction.z * ballMovementSpeed * (f32)deltaTimeSeconds;

    if (IsBallOutOfBounds())
    {
        m_Ball->Position = originalPosition;
        return;
    }

    // Top & bottom wall
    if (Cube::CheckCollision(*m_Ball, *m_Map[m_TopIndex]) || Cube::CheckCollision(*m_Ball, *m_Map[m_BottomIndex]))
    {
        direction.z = -direction.z;
        m_Ball->Position = originalPosition;
        return;
    }

    // Left & right wall
    if (Cube::CheckCollision(*m_Ball, *m_Map[m_LeftIndex]) || Cube::CheckCollision(*m_Ball, *m_Map[m_RightIndex]))
    {
        direction = GetRandomDirection();
        m_Ball->Position.x = 0.0f;
        m_Ball->Position.z = 0.0f;
        m_Ball->SetRandomColor();
        // Add score
        return;
    }

    if (Cube::CheckCollision(*m_Ball, *m_Players[0]))
    {
        if (direction.x < 0.0f)
        {
            direction.x = -direction.x;
            m_Ball->Position = originalPosition;
            m_Ball->SetRandomColor();
            return;
        }
    }

    if (Cube::CheckCollision(*m_Ball, *m_Players[1]))
    {
        if (direction.x > 0.0f)
        {
            direction.x = -direction.x;
            m_Ball->Position = originalPosition;
            m_Ball->SetRandomColor();
            return;
        }
    }
}

void Pong::OnRender()
{
    Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    Renderer::ClearBuffer();

    // Todo: better mutex locking?
    m_Mutex.lock();
    m_Ball->Draw(*m_Camera);
    m_Players[0]->DrawLit(*m_Camera, m_Ball);
    m_Players[1]->DrawLit(*m_Camera, m_Ball);
    for (const auto &item: m_Map)
    {
        item->DrawLit(*m_Camera, m_Ball);
    }
    m_Mutex.unlock();
}

bool Pong::IsBallOutOfBounds() const
{
    return (
        m_Ball->Position.x < m_Map[m_LeftIndex]->Position.x ||
        m_Ball->Position.x > m_Map[m_RightIndex]->Position.x ||
        m_Ball->Position.z < m_Map[m_TopIndex]->Position.z ||
        m_Ball->Position.z > m_Map[m_BottomIndex]->Position.z
    );
}

void Pong::OnImGUIRender()
{
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::Begin("Debug");
    {

    }
    ImGui::End();
}

void Pong::OnResize()
{
    m_Camera->SetViewport(m_Properties.WindowWidth, m_Properties.WindowHeight);
}