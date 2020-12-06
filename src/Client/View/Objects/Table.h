#ifndef BLACKJACK_TABLE_H
#define BLACKJACK_TABLE_H

#include "IGameObject.h"

struct Table : public IGameObject {
public:
    Table(glm::vec2 position, glm::vec2 size, float rotation = 0.f, float layer = -1.f);

    ~Table() = default;

    void SetSprite(std::shared_ptr<Graphic::SpriteAnimator> anim);

    void UpdateSprite(std::shared_ptr<Graphic::Sprite> sprite);

    void SetPosition(glm::vec2 new_pose);

    void Render() override;

private:
    void SetAnimator(VAL card_price, SUIT card_suit, std::shared_ptr<Graphic::SpriteAnimator>) override;
    static inline const VAL m_val = VAL::STOPPER;
    static inline const SUIT m_suit = SUIT::STOPPER;
};


#endif //BLACKJACK_TABLE_H
