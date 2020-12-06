#include "Engine.h"

void Engine::Render() {
    m_shader->use();
    Table->Render();
    Card_Spawner->Render();
    m_cam->Render();
    player_controller->Render();
    for (auto i : Other_players)
        i->Render();
    for (auto i : Cards)
        i->Render();
}

Engine &Engine::Editor() {
    static Engine engine = Engine();
    return engine;
}
