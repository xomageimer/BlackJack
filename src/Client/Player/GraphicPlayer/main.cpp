#undef BOOST_ASIO_ENABLE_HANDLER_TRACKING

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include "graphic_config.h"
#include "OutputManager.h"
#include "TCP_Player_Client.h"

#include "Engine/Engine.h"

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: chat_client <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        auto endpoint_iterator = resolver.resolve({ argv[1], argv[2]});

        std::shared_ptr<OutputManager> manager = std::make_shared<OutputManager>();
        manager->subscribe("Logger", std::make_shared<ConsoleLogger>(std::cout));

        std::shared_ptr<TCP_Player_Client> c = std::make_shared<TCP_Player_Client>(1'000, io_service, endpoint_iterator);
        c->SetManager(manager);

        std::thread t([&io_service](){ io_service.run(); });

        auto window = Graphic_Interface::CreateAndSafeWindow(960, 600, "BLACK JACK");

        Graphic_Interface::Preparation(window, c);

        while(!glfwWindowShouldClose(window))
        {
            Graphic_Interface::processInput(window);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            Engine::Editor().Run();
        }

        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}