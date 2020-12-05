#include "Sprite.h"

Graphic::Sprite::Sprite(std::shared_ptr<Texture2D> texture, std::shared_ptr<Shader> shader) : m_texture(texture), m_shader(shader) {
    this->m_vao.emplace();
}

Graphic::Sprite::Sprite(Graphic::Sprite && other) noexcept {
    this->m_texture = std::move(other.m_texture);
    this->m_shader = std::move(other.m_shader);
    this->m_vao = std::move(other.m_vao);
    this->m_vertices_bo = std::move(other.m_vertices_bo);
    this->m_textures_bo = std::move(other.m_textures_bo);
    this->m_ebo = std::move(other.m_ebo);
}

Graphic::Sprite &Graphic::Sprite::operator=(Graphic::Sprite && other) noexcept {
    if (this == &other)
        return *this;

    this->m_texture = std::move(other.m_texture);
    this->m_shader = std::move(other.m_shader);
    this->m_vao = std::move(other.m_vao);
    this->m_vertices_bo = std::move(other.m_vertices_bo);
    this->m_textures_bo = std::move(other.m_textures_bo);
    this->m_ebo = std::move(other.m_ebo);

    return *this;
}

void Graphic::Sprite::SetSpriteSheet(const std::vector<float> & vec) {
    if (textureCoords != &vec) {
        textureCoords = &vec;
        this->m_vao->bind();
        this->m_textures_bo->Reset(&(*textureCoords)[0], sizeof(float) * textureCoords->size());
    }
}

void Graphic::Sprite::draw() {

    this->m_shader->use();
    this->m_shader->glUniform("texture_", 0);
    this->m_texture->bind();
    this->m_vao->bind();
    this->m_vao->EnableAllAttrib();

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(m_center.x, m_center.y, 0.0));
    trans = glm::scale(trans, glm::vec3(m_size, 0.f));
    trans = glm::rotate(trans, m_rotation, glm::vec3(0.0f, 0.0f, 1.f));

    this->m_shader->glUniform("transform", trans);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Graphic::Sprite::Init(
                              const void *data_vert,
                              size_t size_vert,
                              unsigned int usage_vert,

                              const void *data_indic,
                              size_t size_indic,
                              unsigned int usage_indic
                          ) {

    this->m_vao->bind();
    this->m_vertices_bo.emplace(data_vert, size_vert, usage_vert);
    this->m_ebo.emplace(data_indic, size_indic, usage_indic);
    this->m_vao->SetVertexAttrib();
    static std::vector<float> sizer = {
            1.f, 1.f,
            1.f, 0.f,
            0.f, 0.f,
            0.f, 1.f
    };
    this->m_textures_bo.emplace(&sizer[0], sizeof(float) * sizer.size(), GL_STATIC_DRAW);
    this->m_vao->SetVertexAttrib(1, 2);
    this->m_vao->EnableAllAttrib();
}

