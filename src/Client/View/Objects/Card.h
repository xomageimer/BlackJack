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

    void SetSprite(std::shared_ptr<Graphic::SpriteAnimator> anim);

    void SetCards_Val_Suit(std::vector<float> & v);

    void UpdateSprite(std::shared_ptr<Graphic::Sprite> sprite);

    void SetSpawn(glm::vec2 spawn);

    void Update();
    void Render() override;
private:
    void SetAnimator(VAL card_price, SUIT card_suit, std::shared_ptr<Graphic::SpriteAnimator>) override;

    glm::vec2 my_spawn{};

    float m_deltaTime;
    float m_lastFrame = 0.f;

    static inline const VAL m_value = VAL::STOPPER;
    static inline const SUIT m_suit = SUIT::STOPPER;

    const glm::vec2 * current_target{};
};


#endif //BLACKJACK_CARD_H
