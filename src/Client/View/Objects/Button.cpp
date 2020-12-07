#include "Button.h"

Button::Button(glm::vec2 position, glm::vec2 size) : IGameObject(position, size) {}

void Button::SetSprite(std::shared_ptr<Graphic::SpriteAnimator> anim) {
    this->SetAnimator(m_val, m_suit, anim);
}

void Button::UpdateSprite(std::shared_ptr<Graphic::Sprite> sprite) {
    sprite->GetCenter() = m_position;
    sprite->GetSize() = m_size;
}

void Button::SetPosition(glm::vec2 new_pose) {
    this->m_position = new_pose;
}

void Button::Render() {
    auto it = m_representative[m_val][m_suit];
    UpdateSprite(it->GetSprite());
    it->AnimationUpdate();
}

void Button::SetAnimator(GameCard::Cards::CardPrice v, GameCard::Cards::CardSuit s,
                         std::shared_ptr<Graphic::SpriteAnimator>) {
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


