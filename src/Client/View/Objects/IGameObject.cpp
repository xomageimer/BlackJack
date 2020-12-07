#include "IGameObject.h"

IGameObject::IGameObject(glm::vec2 position, glm::vec2 size)
    : m_position(position),
    m_size(size) {}

std::shared_ptr<Graphic::SpriteAnimator> IGameObject::GetAnimator(VAL value, SUIT suit) {
    return this->m_representative.at(value).at(suit);
}
