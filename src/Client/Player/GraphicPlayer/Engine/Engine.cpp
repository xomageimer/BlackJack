#include "Engine.h"

void Engine::Render() {
    m_shader->use();
    Table->Render();
    Card_Spawner->Render();
    m_cam->Render();
    player_controller->Render();
    for (auto i : Other_players)
        i->Render();
}

Engine &Engine::Editor() {
    static Engine engine = Engine();
    return engine;
}

void Engine::Run() {
    if (player_controller->Is_Authorize())
        player_controller->Process();
    else
        throw std::logic_error("YOU HAVEN'T NAME!");
}

void Engine::ConfigPlayer(const std::string &sprite_name) {
    player_controller->SetSprite(std::make_shared<Graphic::SpriteAnimator>(pool_sprites.find(sprite_name)->second, std::vector<std::vector<float>>{{0.999f, 0.999f, 0.999f, 0.011f, 0.011f, 0.011f, 0.011f, 0.999f}}));
}

void Engine::ConfigCards(GameCard::Cards::CardPrice value, GameCard::Cards::CardSuit suit,
                         glm::vec2 left_bottom,
                         glm::vec2 right_top,
                         float x,
                         float y,
                         size_t vert_from,
                         size_t vertical,
                         size_t horizontal) {
    for (size_t i = vert_from; i < vertical; i++){
        auto vert_bottom_y = left_bottom.y - y * i;
        auto vert_top_y = right_top.y - y * i;
        for (size_t j = 0; j < horizontal; j++){
            card_sprites[suit][++value] =
                    std::vector<float>{right_top.x + x * j,   vert_top_y,
                                       right_top.x + x * j,   vert_bottom_y,
                                       left_bottom.x + x * j,  vert_bottom_y,
                                       left_bottom.x + x * j, vert_top_y}
                                       ;
        }
    }
}

void Engine::SetCardVelocity(float new_velocity) {
    for (auto & i : Cards){
        i->setVelocity(new_velocity);
    }
}

void Engine::SetTable(glm::vec2 position, glm::vec2 size) {
    this->Table = std::make_shared<struct Table>(position, size);
}

void Engine::ConfigSpriteTable(const std::string &sprite_name) {
    this->Table->SetSprite(std::make_shared<Graphic::SpriteAnimator>(pool_sprites.find(sprite_name)->second, std::vector<std::vector<float>>{{0.999f, 0.999f, 0.999f, 0.011f, 0.011f, 0.011f, 0.011f, 0.999f}}));
}


void Engine::SetButton(glm::vec2 position, glm::vec2 size) {
    this->butt_ok = std::make_shared<Button>(position, size);
    this->butt_stand = std::make_shared<Button>(position, size);
    this->butt_double = std::make_shared<Button>(position, size);
    this->butt_10 = std::make_shared<Button>(position, size);
    this->butt_50 = std::make_shared<Button>(position, size);
    this->butt_100 = std::make_shared<Button>(position, size);
}

void Engine::ConfigSpriteButton(const butt_type &b, const std::string &sprite_name) {
    switch(b) {
        case butt_type::OK :
            this->butt_ok->SetSprite(std::make_shared<Graphic::SpriteAnimator>(pool_sprites.find(sprite_name)->second, std::vector<std::vector<float>>{{0.999f, 0.999f, 0.999f, 0.011f, 0.011f, 0.011f, 0.011f, 0.999f}}));
            break;
        case butt_type::STAND :
            this->butt_stand->SetSprite(std::make_shared<Graphic::SpriteAnimator>(pool_sprites.find(sprite_name)->second, std::vector<std::vector<float>>{{0.999f, 0.999f, 0.999f, 0.011f, 0.011f, 0.011f, 0.011f, 0.999f}}));
            break;
        case butt_type::DOUBLE :
            this->butt_double->SetSprite(std::make_shared<Graphic::SpriteAnimator>(pool_sprites.find(sprite_name)->second, std::vector<std::vector<float>>{{0.999f, 0.999f, 0.999f, 0.011f, 0.011f, 0.011f, 0.011f, 0.999f}}));
            break;
        case butt_type::_10 :
            this->butt_10->SetSprite(std::make_shared<Graphic::SpriteAnimator>(pool_sprites.find(sprite_name)->second, std::vector<std::vector<float>>{{0.999f, 0.999f, 0.999f, 0.011f, 0.011f, 0.011f, 0.011f, 0.999f}}));
            break;
        case butt_type::_50 :
            this->butt_50->SetSprite(std::make_shared<Graphic::SpriteAnimator>(pool_sprites.find(sprite_name)->second, std::vector<std::vector<float>>{{0.999f, 0.999f, 0.999f, 0.011f, 0.011f, 0.011f, 0.011f, 0.999f}}));
            break;
        case butt_type::_100 :
            this->butt_100->SetSprite(std::make_shared<Graphic::SpriteAnimator>(pool_sprites.find(sprite_name)->second, std::vector<std::vector<float>>{{0.999f, 0.999f, 0.999f, 0.011f, 0.011f, 0.011f, 0.011f, 0.999f}}));
            break;
        default:
            throw std::logic_error("BUTTON NOT FOUND!");
            break;
    }
}

void Engine::SetStack(glm::vec2 position, glm::vec2 size) {
    this->Card_Spawner = std::make_shared<struct Stack>(position, size);
    for (auto & i : Cards){
        i->SetSpawn(position);
    }
}

void Engine::ConfigSpriteStack(const std::string &sprite_name) {
    this->Card_Spawner->SetSprite(std::make_shared<Graphic::SpriteAnimator>(pool_sprites.find(sprite_name)->second, std::vector<std::vector<float>>{{0.999f, 0.999f, 0.999f, 0.011f, 0.011f, 0.011f, 0.011f, 0.999f}}));
}

void Engine::SetSprite(const std::string &name, const std::string &texture_name) {
    auto it_texture = pool_textures.find(texture_name);
    if (it_texture != pool_textures.end()) {
        pool_sprites.emplace(name, std::make_shared<Graphic::Sprite>(it_texture->second, m_shader));
    }
}

void Engine::SetSpritePack(const std::string &name, const std::string &texture_name, size_t max_count) {
    pool_pack_sprites[name].reserve(max_count);
    auto it_texture = pool_textures.find(texture_name);

    if (it_texture != pool_textures.end())
        for (size_t i = 0; i < max_count; i++)
            pool_pack_sprites[name].emplace_back(std::make_shared<Graphic::Sprite>(it_texture->second, m_shader));

}

void Engine::SetTexture(const std::string &name, const std::filesystem::path & path_to_texture, unsigned int type, unsigned int format,
                        bool flip_vertically) {
    pool_textures.emplace(name, std::make_shared<Graphic::Texture2D>(path_to_texture, type, format, flip_vertically));
}

void Engine::SetShader(const std::filesystem::path &vertex_shader, const std::filesystem::path &fragment_shader) {
    m_shader = std::make_shared<Shader>(vertex_shader, fragment_shader);
    m_cam = std::make_shared<Camera>(m_shader);
}

void Engine::SetPlayerCard(GameCard::Cards::CardPrice value, GameCard::Cards::CardSuit suit) {
    auto card = Cards[current_card++];
    card->SetCards_Val_Suit(card_sprites[suit][value]);
    card->SetPlayerTarget(player_controller->GetCurrentPosition());
    player_controller->Set_Card_Sprite(card);
}

void Engine::SetCard(GameCard::Cards::CardPrice value, GameCard::Cards::CardSuit suit, size_t player_number) {
    auto card= Cards[current_card++];
    card->SetCards_Val_Suit(card_sprites[suit][value]);
    card->SetPlayerTarget(Other_players[player_number]->GetCurrentPosition());
    Other_players[player_number]->Set_Card_Sprite(card);
}

void Engine::CreateCards(glm::vec2 size, size_t numbers, const std::string & sprite_name) {
    auto it = pool_pack_sprites.find(sprite_name);
    size_t j = 0;
    if (it == pool_pack_sprites.end()) return;
    Cards.reserve(numbers);
    for(size_t i = 0; i < numbers; i++){
        Cards.emplace_back(std::make_shared<Card>(glm::vec2{0.f, 0.f}, size));
        Cards.back()->SetSprite(std::make_shared<Graphic::SpriteAnimator>(it->second[j++], std::vector<std::vector<float>>{{0.999f, 0.999f, 0.999f, 0.011f, 0.011f, 0.011f, 0.011f, 0.999f}}));
    }
}

void Engine::ClearRound() {
    player_controller->Clear();
    for (auto & i : Other_players){
        i->Clear();
    }
    current_card = 0;
}