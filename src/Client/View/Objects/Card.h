#ifndef BLACKJACK_CARD_H
#define BLACKJACK_CARD_H

#include "Objects/IGameObject.h"

struct Card : public IGameObject {
public:
    Card(glm::vec2 position, glm::vec2 size);
    Card(const Card &) = default;
    Card & operator=(const Card &) = default;
    Card(Card &&) = default;
    Card & operator=(Card &&) = default;

    inline void SetPlayerTarget(const glm::vec2 & targ){
        current_target = &targ;
    };

    void SetCards_Val_Suit(VAL value, SUIT suit);

    void SetAnimator(VAL card_price, SUIT card_suit, std::shared_ptr<Graphic::SpriteAnimator>) override;
    void UpdateSprite(std::shared_ptr<Graphic::Sprite> sprite);

    void SetSpawn(glm::vec2 spawn);

    void Update();
    void Render() override;
private:
    glm::vec2 my_spawn{};

    float m_deltaTime;
    float m_lastFrame = 0.f;

    VAL m_value = VAL::ACE;
    SUIT m_suit = SUIT::DIAMONDS;

    const glm::vec2 * current_target{};
};


#endif //BLACKJACK_CARD_H
