#include "Card.h"

Card::Card(glm::vec2 position, glm::vec2 size) : IGameObject(position, size) {}

void Card::SetAnimator(GameCard::Cards::CardPrice card_price, GameCard::Cards::CardSuit card_suit,
                       std::shared_ptr<Graphic::SpriteAnimator>) {

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
