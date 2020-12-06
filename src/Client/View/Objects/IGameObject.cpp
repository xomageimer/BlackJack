#include "IGameObject.h"

IGameObject::IGameObject(glm::vec2 position, glm::vec2 size, float rotation, float layer)
    : m_position(position),
    m_size(size),
    m_rotation(rotation),
    m_layer(layer)
{

}

std::shared_ptr<Graphic::SpriteAnimator> IGameObject::GetAnimator(VAL value, SUIT suit) {
    return this->m_representative.at(value).at(suit);
}
