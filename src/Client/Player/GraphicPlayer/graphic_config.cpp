#include "graphic_config.h"

#include <iostream>
#include <exception>

#include "Client/Player/GraphicPlayer/Engine/Engine.h"

GLFWwindow* Graphic_Interface::CreateAndSafeWindow(const size_t WIDTH, const size_t HEIGHT, std::string PROJECT_NAME) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, PROJECT_NAME.c_str(), NULL, NULL);

    if (!window){
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        throw std::bad_exception();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr <<  "Failed to init GLAD"  << std::endl;
        throw std::bad_exception();
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return window;
}

void Graphic_Interface::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Graphic_Interface::processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        Engine::Editor().GetPlayerController()->keyboard_controller(window, deltaTime, lastFrame);
}
