#ifndef BLACKJACK_ENGINE_H
#define BLACKJACK_ENGINE_H

#include <memory>
#include <map>
#include <string>
#include <utility>

#include "Graphic/Sprite.h"
#include "Graphic/Shader.h"
#include "Graphic/SpriteAnimator.h"
#include "Camera.h"

#include "Client/Player/GraphicPlayer/TCP_Player_Client.h"
#include "Objects/Net_Player.h"
#include "Objects/Table.h"
#include "Objects/Card.h"
#include "Objects/Stack.h"

enum class butt_type{
    OK,
    STAND,
    DOUBLE,
    _10,
    _50,
    _100
};

struct Engine {
public:
    static Engine& Editor();
    void Render();
    void Run();
    Engine(Engine&) = delete;
    Engine& operator=(Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

    [[nodiscard]] inline std::shared_ptr<TCP_Player_Client> & GetPlayerController() {
        return player_controller;
    }
    inline void SetPlayer(std::shared_ptr<TCP_Player_Client> player, glm::vec2 position, glm::vec2 size){
        player_controller.reset();
        player_controller = std::move(player);
        player_controller->SetPosition(position);
        player_controller->SetSize(size);
    };
    void ConfigPlayer(const std::string & sprite_name);

    void ConfigCard(std::shared_ptr<Card> p, VAL value, SUIT suit, const std::string & sprite_name, const std::vector<std::vector<float>> & sprite_frames);
    void ConfigCards(VAL value, SUIT suit, const std::string & sprite_name, glm::vec2 left_bottom,
                     glm::vec2 right_top,
                     float x,
                     float y,
                     size_t vertical,
                     size_t horizontal);
    void SetCardVelocity(float new_velocity);

    void SetTable(glm::vec2 position, glm::vec2 size);
    void ConfigSpriteTable(const std::string & sprite_name);

    void SetButton(glm::vec2 position, glm::vec2 size);
    void ConfigSpriteButton(const butt_type & b, const std::string & sprite_name);

    void SetStack(glm::vec2 position, glm::vec2 size); // TODO тут же и задать спавн карт
    void ConfigSpriteStack(const std::string & sprite_name);

    void SetSprite(const std::string & name,
                   const std::string & texture_name);

    void SetSpritePack(const std::string & name,
                       const std::string & texture_name, size_t max_count);

    void SetTexture(const std::string& name,
                    const std::filesystem::path &,

                    unsigned int type = GL_RGBA,
                    unsigned int format = GL_RGBA,
                    bool flip_vertically = true);

    void SetShader(const std::filesystem::path & vertex_shader, const std::filesystem::path & fragment_shader);

private:
    Engine() = default;

    std::shared_ptr<Shader> m_shader;

    std::shared_ptr<TCP_Player_Client> player_controller = nullptr;

    std::vector<std::shared_ptr<Net_Player>> Other_players;

    std::shared_ptr<Stack> Card_Spawner;
    std::shared_ptr<Table> Table;

    std::vector<std::shared_ptr<Card>> Cards;

    std::shared_ptr<Button> butt_ok;
    std::shared_ptr<Button> butt_stand;
    std::shared_ptr<Button> butt_double;

    std::shared_ptr<Button> butt_10;
    std::shared_ptr<Button> butt_50;
    std::shared_ptr<Button> butt_100;

    std::map<std::string, std::shared_ptr<Graphic::Sprite>> pool_sprites;
    std::map<std::string, std::vector<std::shared_ptr<Graphic::Sprite>>> pool_pack_sprites;
    std::map<std::string, std::shared_ptr<Graphic::Texture2D>> pool_textures;

    std::shared_ptr<Camera> m_cam;
};


#endif //BLACKJACK_ENGINE_H
