#ifndef GAMEENGINE_SPRITEANIMATOR_H
#define GAMEENGINE_SPRITEANIMATOR_H

// TODO класс обработчик анимации (по факту должен как-то итерироваться по одному изображению с кучей шитов (между этими шитами))

#include <memory>
#include "Sprite.h"

namespace Graphic {

    struct SpriteAnimator {
    public:
        explicit SpriteAnimator(std::shared_ptr<Sprite> sprite, const std::vector<std::vector<float>> &);
        ~SpriteAnimator() = default;

        SpriteAnimator(const SpriteAnimator &) = default;
        SpriteAnimator& operator=(const SpriteAnimator &) = default;

        void SetSheet(const glm::vec2 &left_bottom, const glm::vec2 &right_top);

        void SetSheetAtlas(glm::vec2 left_bottom, glm::vec2 right_top, float x,
                           float y, size_t vertical, size_t horizontal);

        void AnimationUpdate();

        void ClearAnimPack();

        [[nodiscard]] inline const std::vector<std::vector<float>> & GetAnim() const {
            return animation;
        }
        [[nodiscard]] inline std::shared_ptr<Sprite> GetSprite() {
            return m_sprite;
        }
        inline void SetSpeed(float new_speed) {
            speed = new_speed;
        }

    private:
        float prev_anim = 0.f;
        float speed = 0.05f;
        size_t current_sheet = 0;

        std::shared_ptr<Sprite> m_sprite;

        std::vector<std::vector<float>> animation;
    };

}

#endif //GAMEENGINE_SPRITEANIMATOR_H
