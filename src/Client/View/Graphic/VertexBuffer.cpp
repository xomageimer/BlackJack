#include "VertexBuffer.h"

Graphic::VertexBuffer::VertexBuffer(const void *data,
                                    size_t size,
                                    unsigned int usage){
    glGenBuffers(1, &ID);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

Graphic::VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &ID);
}

Graphic::VertexBuffer::VertexBuffer(Graphic::VertexBuffer && other) noexcept{
    this->ID = other.ID;
    other.ID = 0;
}

Graphic::VertexBuffer &Graphic::VertexBuffer::operator=(Graphic::VertexBuffer && other) noexcept{
    this->ID = other.ID;
    other.ID = 0;

    return *this;
}

void Graphic::VertexBuffer::Reset(const void *data, size_t size) {
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void Graphic::VertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void Graphic::VertexBuffer::unbind() {
    glBindBuffer(0, ID);

}

