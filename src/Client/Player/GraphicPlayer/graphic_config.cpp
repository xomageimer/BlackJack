#include "graphic_config.h"

#include <iostream>
#include <exception>
#include <utility>

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
    glfwSetCursorPosCallback(window, mouse_callback);

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
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        Engine::Editor().GetPlayerController()->close();
}

void Graphic_Interface::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    Engine::Editor().GetPlayerController()->mouse_controller(window, xpos, ypos);
}

void Graphic_Interface::Preparation(GLFWwindow *window, std::shared_ptr<TCP_Player_Client> player) {
    std::fstream config_file;
    config_file.open(((std::filesystem::current_path())/"res"/"nickname.txt").u8string());
    std::string name;
    if (config_file.is_open()) {
        std::stringstream ss;
        ss << config_file.rdbuf();
        name = ss.str();
    } else {
        name = "noname";
    }

    player->SetName(name);

    Engine::Editor().SetPlayer(std::move(player), {0.f, 0.f}, {0.13f, 0.2f});

    auto Resource_Path = (std::filesystem::current_path())/"res";
    auto Shaders_Path = (std::filesystem::current_path())/"shaders";

    Engine::Editor().SetShader(Shaders_Path/"vertex_shader.glsl", Shaders_Path/"fragment_shader.glsl");

    Engine::Editor().SetTexture("Current_Player_Texture", (Resource_Path/"textures/cur_player.png"));
    Engine::Editor().SetTexture("Other_Player_Texture", (Resource_Path/"textures/player.png"));
    Engine::Editor().SetTexture("Card_Texture", (Resource_Path/"textures/cards.png"));
    Engine::Editor().SetTexture("Table_Texture", (Resource_Path/"textures/background.jpg"), GL_RGB);
    Engine::Editor().SetTexture("Stack_Texture", (Resource_Path/"textures/stack.png"));
    Engine::Editor().SetTexture("Xchips_Texture", (Resource_Path/"textures/X.png"));
    Engine::Editor().SetTexture("10chips_Texture", (Resource_Path/"widget/10.png"));
    Engine::Editor().SetTexture("50chips_Texture", (Resource_Path/"widget/50.png"));
    Engine::Editor().SetTexture("100chips_Texture", (Resource_Path/"widget/100.png"));
    Engine::Editor().SetTexture("Ok_Texture", (Resource_Path/"widget/yes.png"));
    Engine::Editor().SetTexture("Stand_Texture", (Resource_Path/"widget/stand.png"));
    Engine::Editor().SetTexture("Double_Texture", (Resource_Path/"widget/double.png"));

    Engine::Editor().SetSprite("Current_Player", "Current_Player_Texture");
    Engine::Editor().SetSpritePack("Other_Player", "Other_Player_Texture", 6);
    Engine::Editor().SetSpritePack("Card", "Card_Texture", 70);
    Engine::Editor().SetSprite("Table", "Table_Texture");
    Engine::Editor().SetSpritePack("Xchips", "Xchips_Texture", 6);
    Engine::Editor().SetSprite("10chips", "Stack_Texture");
    Engine::Editor().SetSprite("50chips", "Stack_Texture");
    Engine::Editor().SetSprite("100chips", "Stack_Texture");
    Engine::Editor().SetSprite("Stack", "Stack_Texture");
    Engine::Editor().SetSprite("Ok", "Ok_Texture");
    Engine::Editor().SetSprite("Stand", "Stand_Texture");
    Engine::Editor().SetSprite("Double", "Double_Texture");

}
