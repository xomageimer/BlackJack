#ifndef BLACKJACK_GRAPHIC_CONFIG_H
#define BLACKJACK_GRAPHIC_CONFIG_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include <string>

namespace Graphic_Interface {
    void framebuffer_size_callback(GLFWwindow*, int, int);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void processInput(GLFWwindow *window);
    GLFWwindow *CreateAndSafeWindow(const size_t WIDTH = 960, const size_t HEIGHT = 600, std::string PROJECT_NAME = "BLACK JACK");
}

#endif //BLACKJACK_GRAPHIC_CONFIG_H
