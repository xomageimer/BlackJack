#include "Net_Player.h"

Net_Player::Net_Player(glm::vec2 position, glm::vec2 size) : IGameObject(position, size) {}

void Net_Player::SetCard(const GameCard::Cards &card) {
    m_hand.SetNewCard(card);
}

void Net_Player::SetAnimator(GameCard::Cards::CardPrice v, GameCard::Cards::CardSuit s,
                             std::shared_ptr<Graphic::SpriteAnimator> anim) {
    auto it = this->m_representative[v].emplace(std::piecewise_construct, std::forward_as_tuple(s), std::forward_as_tuple(anim));
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
    it.first->second->GetSprite()->Init(vertices, sizeof(vertices), GL_DYNAMIC_DRAW, indices, sizeof(indices), GL_DYNAMIC_DRAW);

}

void Net_Player::SetSprite(std::shared_ptr<Graphic::SpriteAnimator> anim) {
    this->SetAnimator(m_val, m_suit, anim);
}

void Net_Player::Set_Card_Sprite(std::shared_ptr<Card> new_card) {
    cards.emplace_back(std::move(new_card));
    cards.back()->GetSize() = card_size;
    cards.back()->GetCurrentPosition() = card_pose;
    cards.back()->SetPlayerTarget(m_position);
}

void Net_Player::SetPosition(glm::vec2 new_pose) {
    m_position = new_pose;
    for (auto & i : cards){
        i->SetPlayerTarget(m_position);
    }
}

void Net_Player::Clear() {
    m_hand.Clear();
    cards.clear();
}

void Net_Player::Render() {
    for (auto & card : cards) {
        card->Render();
    }
    auto it = m_representative.at(m_val).at(m_suit);
    UpdateSprite(it->GetSprite());
    it->AnimationUpdate();
}

void Net_Player::UpdateSprite(std::shared_ptr<Graphic::Sprite> sprite) {
    sprite->GetCenter() = m_position;
    sprite->GetSize() = m_size;
}

void Net_Player::SetSize(glm::vec2 new_size) {
    m_size = new_size;
}

