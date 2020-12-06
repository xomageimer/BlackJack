#ifndef BLACKJACK_ENGINE_H
#define BLACKJACK_ENGINE_H

#include <memory>
#include <map>
#include <string>

#include "Graphic/Sprite.h"
#include "Graphic/Shader.h"
#include "Graphic/SpriteAnimator.h"
#include "Camera.h"

#include "Client/Player/GraphicPlayer/TCP_Player_Client.h"
#include "Objects/Net_Player.h"
#include "Objects/Table.h"
#include "Objects/Card.h"
#include "Objects/Stack.h"

struct Engine {
public:
    static Engine& Editor();
    void Render();
    Engine(Engine&) = delete;
    Engine& operator=(Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

    [[nodiscard]] inline std::shared_ptr<TCP_Player_Client> & GetPlayerController() {
        return player_controller;
    }

private:
    Engine() = default;

    std::shared_ptr<Shader> m_shader;

    std::shared_ptr<TCP_Player_Client> player_controller = nullptr;

    std::vector<std::shared_ptr<Net_Player>> Other_players;
    std::vector<std::shared_ptr<Card>> Cards;
    std::shared_ptr<Stack> Card_Spawner;
    std::shared_ptr<Table> Table;

    std::map<std::string, std::shared_ptr<Graphic::Sprite>> pool_sprites;
    std::map<std::string, std::vector<std::shared_ptr<Graphic::Sprite>>> pool_pack_sprites;
    std::map<std::string, std::shared_ptr<Graphic::Texture2D>> pool_textures;

    std::shared_ptr<Camera> m_cam;
};


#endif //BLACKJACK_ENGINE_H
