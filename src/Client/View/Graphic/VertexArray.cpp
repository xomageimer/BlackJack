#include "VertexArray.h"

Graphic::VertexArray::VertexArray() : m_last_attrib(0) {
    glGenVertexArrays(1, &ID);
    bind();
}

Graphic::VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &ID);
}

Graphic::VertexArray::VertexArray(Graphic::VertexArray && other) noexcept{
    this->ID = other.ID;
    other.ID = 0;
    this->m_last_attrib = other.m_last_attrib;
}

Graphic::VertexArray &Graphic::VertexArray::operator=(Graphic::VertexArray && other) noexcept {
    this->ID = other.ID;
    other.ID = 0;
    this->m_last_attrib = other.m_last_attrib;

    return *this;
}

size_t Graphic::VertexArray::SetVertexAttrib(size_t index,
                                             size_t size,
                                             size_t stride,
                                             unsigned int type,
                                             bool normalized,
                                             const void *offset) {
    glVertexAttribPointer((index >= m_last_attrib) ? index : m_last_attrib,
                          size,
                          type,
                          normalized,
                          stride,
                          offset
                          );
    return ++m_last_attrib;
}

void Graphic::VertexArray::bind() {
    glBindVertexArray(ID);
}

void Graphic::VertexArray::unbind() {
    glBindVertexArray(0);
}

void Graphic::VertexArray::EnableAllAttrib() {
    for (size_t i = 0; i < m_last_attrib; i++){
        glEnableVertexAttribArray(i);
    }
}

void Graphic::VertexArray::DisableAllAttrib() {
    for (size_t i = 0; i < m_last_attrib; i++){
        glDisableVertexAttribArray(i);
    }
}

bool Graphic::VertexArray::EnableAttrib(size_t i) {
    if (i < m_last_attrib)
        glEnableVertexAttribArray(i);
    else
        return false;
    return true;
}


bool Graphic::VertexArray::DisableAttrib(size_t i) {
    if (i < m_last_attrib)
        glDisableVertexAttribArray(i);
    else
        return false;
    return true;
}











