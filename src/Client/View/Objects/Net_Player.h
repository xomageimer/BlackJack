#ifndef BLACKJACK_NET_PLAYER_H
#define BLACKJACK_NET_PLAYER_H

#include "IGameObject.h"
#include "Cards/CardStack.h"
#include "Card.h"

struct Net_Player : public IGameObject {
    Net_Player(glm::vec2 position, glm::vec2 size);
    ~Net_Player() override = default;

    void SetSprite(std::shared_ptr<Graphic::SpriteAnimator> anim);

    void Set_Card_Sprite(std::shared_ptr<Card>);
    void SetCard(const GameCard::Cards & card);
    void Clear();

    void Render() override;
    void UpdateSprite(std::shared_ptr<Graphic::Sprite> sprite);

    void SetPosition(glm::vec2 new_pose);
    void SetSize(glm::vec2 new_size);
protected:
    GameCard::Hand m_hand; // TODO для сравнения с картами из PlayerChanged

    void SetAnimator(VAL card_price, SUIT card_suit, std::shared_ptr<Graphic::SpriteAnimator>) override;

    static inline const VAL m_val = VAL::STOPPER;
    static inline const SUIT m_suit = SUIT::STOPPER;

    std::vector<std::shared_ptr<Card>> cards;
};


#endif //BLACKJACK_NET_PLAYER_H
