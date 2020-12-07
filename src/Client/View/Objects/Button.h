#ifndef BLACKJACK_BUTTON_H
#define BLACKJACK_BUTTON_H

#include "IGameObject.h"

struct Button : public IGameObject {
public:
    Button(glm::vec2 position, glm::vec2 size);
    Button(const Button&) = default;
    Button& operator=(const Button&) = default;
    Button(Button &&) = default;
    Button& operator=(Button &&) = default;

    ~Button() = default;

    void SetSprite(std::shared_ptr<Graphic::SpriteAnimator> anim);

    void UpdateSprite(std::shared_ptr<Graphic::Sprite> sprite);

    void SetPosition(glm::vec2 new_pose);

    void Render() override;

private:
    void SetAnimator(VAL card_price, SUIT card_suit, std::shared_ptr<Graphic::SpriteAnimator>) override;

    static inline const VAL m_val = VAL::STOPPER;
    static inline const SUIT m_suit = SUIT::STOPPER;
};


#endif //BLACKJACK_BUTTON_H
