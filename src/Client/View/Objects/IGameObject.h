#ifndef BLACKJACK_IGAMEOBJECT_H
#define BLACKJACK_IGAMEOBJECT_H

#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Graphic/SpriteAnimator.h"
#include "Cards/CardStack.h"

#define VAL GameCard::Cards::CardPrice
#define SUIT GameCard::Cards::CardSuit

struct IGameObject {
public:
    IGameObject(glm::vec2 position, glm::vec2 size);

    virtual ~IGameObject() = default;

    virtual void SetAnimator(VAL card_price, SUIT card_suit, std::shared_ptr<Graphic::SpriteAnimator>) = 0; // TODO мб вырезать непосредственно в карты
    virtual std::shared_ptr<Graphic::SpriteAnimator> GetAnimator(VAL value, SUIT suit);

    virtual void Render() = 0;

    virtual inline void setVelocity(float vel) {
        m_velocity = vel;
    }

    [[nodiscard]] virtual inline glm::vec2 &GetCurrentPosition() {
        return m_position;
    };

    [[nodiscard]] virtual inline glm::vec2 &GetCurrentDirection() {
        return m_direction;
    }

    [[nodiscard]] virtual inline glm::vec2 &GetSize() {
        return m_size;
    }

    [[nodiscard]] virtual inline float &velocity() {
        return m_velocity;
    }

protected:
    std::map<VAL, std::map<SUIT, std::shared_ptr<Graphic::SpriteAnimator>>> m_representative;

    glm::vec2 m_position;
    glm::vec2 m_size;

    glm::vec2 m_direction{};
    glm::vec3 m_up{0.f, 0.f, 1.f};
    float m_velocity{};
};


#endif //BLACKJACK_IGAMEOBJECT_H
