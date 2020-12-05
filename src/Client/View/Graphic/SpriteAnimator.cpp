#include "SpriteAnimator.h"

Graphic::SpriteAnimator::SpriteAnimator(std::shared_ptr<Sprite> sprite, const std::vector<std::vector<float>> & animPack) : m_sprite(sprite), animation(animPack)
{}


void Graphic::SpriteAnimator::AnimationUpdate() {
    auto cur_time = static_cast<float>(glfwGetTime());
    if (cur_time - prev_anim >= speed) {
        m_sprite->SetSpriteSheet(animation[current_sheet++]);
        current_sheet = (current_sheet == animation.size()) ? 0 : current_sheet;
        prev_anim = cur_time;
    }
    m_sprite->draw();
}

void Graphic::SpriteAnimator::SetSheet(const glm::vec2 &left_bottom, const glm::vec2 &right_top) {
    animation.emplace_back(
            std::vector<float>{right_top.x,   right_top.y,
                               right_top.x,   left_bottom.y,
                               left_bottom.x, left_bottom.y,
                               left_bottom.x, right_top.y}
                               );
}

void Graphic::SpriteAnimator::ClearAnimPack() {
    animation.clear();
}

void Graphic::SpriteAnimator::SetSheetAtlas(glm::vec2 left_bottom,
                                            glm::vec2 right_top,
                                            float x,
                                            float y,
                                            size_t vertical,
                                            size_t horizontal) {
    ClearAnimPack();
    for (size_t i = 0; i < vertical; i++){
        auto vert_bottom_y = left_bottom.y - y * i;
        auto vert_top_y = right_top.y - y * i;
        for (size_t j = 0; j < horizontal; j++){
            SetSheet({left_bottom.x + x * j, vert_bottom_y}, {right_top.x + x * j, vert_top_y});
        }
    }
}



