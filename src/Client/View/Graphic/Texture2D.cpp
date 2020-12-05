#include "Texture2D.h"

Graphic::Texture2D::Texture2D(const std::filesystem::path & path_to_image, unsigned int type, unsigned int format, bool flip_vertically) {
    stbi_set_flip_vertically_on_load(flip_vertically);
    this->m_data = stbi_load(path_to_image.u8string().c_str(), &m_width, &m_height, &m_nrChannels, 0);

    if (!m_data) {
        std::cerr << "Image file path: " << path_to_image << std::endl;
        std::cerr << "ERROR::IMAGE_SOURCE::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return;
    }

    glGenTextures(1, &ID);
    bind();

    SetMode(GL_TEXTURE_WRAP_S);
    SetMode(GL_TEXTURE_WRAP_T);
    SetFilter(GL_TEXTURE_MIN_FILTER);
    SetFilter(GL_TEXTURE_MAG_FILTER);

    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, type, GL_UNSIGNED_BYTE, m_data);

    FreeTrash();
}

Graphic::Texture2D::~Texture2D() {
    glDeleteTextures(1, &ID);
}

void Graphic::Texture2D::SetMode(unsigned int wrap_coordinate, unsigned int new_mode, const std::vector<float> &color) {
    if (new_mode == GL_CLAMP_TO_BORDER)
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &color[0]);
    glTexParameteri(GL_TEXTURE_2D, wrap_coordinate, new_mode);
}

void Graphic::Texture2D::SetFilter(unsigned int type_filter, unsigned int new_filter) {
    glTexParameteri(GL_TEXTURE_2D, type_filter, new_filter);
}

void Graphic::Texture2D::bind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Graphic::Texture2D::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphic::Texture2D::FreeTrash() {
    stbi_image_free(m_data);
}

void Graphic::Texture2D::GenerateMipmap() {
    glGenerateMipmap(GL_TEXTURE_2D);
}

Graphic::Texture2D::Texture2D(Graphic::Texture2D && other) noexcept {
    this->ID = other.ID;
    this->m_width = other.m_width;
    this->m_height = other.m_height;
    this->m_nrChannels = other.m_nrChannels;
    std::swap(this->m_data, other.m_data);
    other.ID = 0;
}

Graphic::Texture2D &Graphic::Texture2D::operator=(Graphic::Texture2D && other) noexcept {
    if (this == &other)
        return *this;

    this->ID = other.ID;
    this->m_width = other.m_width;
    this->m_height = other.m_height;
    this->m_nrChannels = other.m_nrChannels;
    std::swap(this->m_data, other.m_data);
    other.ID = 0;
    return *this;
}


