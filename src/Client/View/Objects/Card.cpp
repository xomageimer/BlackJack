#include "Card.h"

Card::Card(glm::vec2 position, glm::vec2 size) : IGameObject(position, size) {}

void Card::SetAnimator(GameCard::Cards::CardPrice v, GameCard::Cards::CardSuit s,
                       std::shared_ptr<Graphic::SpriteAnimator> anim) {
    auto it = this->m_representative.emplace(std::piecewise_construct, std::forward_as_tuple(v), std::forward_as_tuple(std::pair{s, anim}));
    float vertices[] = {
            1.f, 1.f, 0.f,
            1.f, -1.f, 0.f,
            -1.f, -1.f, 0.f,
            -1.f, 1.f, 0.f
    };
    const int indices[] {
            0, 1, 3,
            1, 2, 3
    };
    it.first->second.at(s)->GetSprite()->Init(vertices, sizeof(vertices), GL_DYNAMIC_DRAW, indices, sizeof(indices), GL_DYNAMIC_DRAW);
}

void Card::SetSpawn(glm::vec2 spawn) {
    my_spawn = spawn;
}

void Card::Update() {
    float currentFrame = glfwGetTime();
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;
    m_direction = glm::normalize(*current_target - m_position);
    if (!(std::abs(current_target->x - m_position.x) <= 0.01f && std::abs(current_target->y - m_position.y) <= 0.01f)) {
        m_position += (m_direction * m_velocity * m_deltaTime);
    }
}

void Card::Render() {
    auto it = m_representative.at(m_value).at(m_suit);
    UpdateSprite(it->GetSprite());
    it->AnimationUpdate();
}

void Card::SetCards_Val_Suit(GameCard::Cards::CardPrice value, GameCard::Cards::CardSuit suit) {
    m_value = value;
    m_suit = suit;
}

void Card::UpdateSprite(std::shared_ptr<Graphic::Sprite> sprite) {
    sprite->GetCenter() = m_position;
    sprite->GetSize() = m_size;
}
