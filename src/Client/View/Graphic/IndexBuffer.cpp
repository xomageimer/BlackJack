#include "IndexBuffer.h"

Graphic::IndexBuffer::IndexBuffer(const void *data, size_t size, unsigned int usage) {
    glGenBuffers(1, &ID);
    bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}

void Graphic::IndexBuffer::Reset(const void *data, size_t size) {
    bind();

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
}

void Graphic::IndexBuffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void Graphic::IndexBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Graphic::IndexBuffer::IndexBuffer(Graphic::IndexBuffer && other) noexcept {
    this->ID = other.ID;
    m_numbers = other.m_numbers;
    other.ID = 0;
}

Graphic::IndexBuffer &Graphic::IndexBuffer::operator=(Graphic::IndexBuffer && other) noexcept {
    this->ID = other.ID;
    m_numbers = other.m_numbers;
    other.ID = 0;

    return *this;
}

Graphic::IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &ID);
}


