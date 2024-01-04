#include "Pong.hpp"

int main()
{
    try
    {
        ApplicationProperties appProperties = {};
        appProperties.WindowTitle = "Multiplayer Pong";
        appProperties.WindowWidth = 1280;
        appProperties.WindowHeight = 960;
        appProperties.RendererBackendAPI = RendererBackendAPI::OpenGL;
        appProperties.WithImGUI = false;

        std::unique_ptr<Application> app = std::make_unique<Pong>(appProperties);
        app->Run();
    }
    catch (const std::exception& e)
    {
        BLAZE_ERROR("{}", e.what());
    }

    return 0;
}