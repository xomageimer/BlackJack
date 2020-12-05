#ifndef GAMEENGINE_INDEXBUFFER_H
#define GAMEENGINE_INDEXBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Graphic {
    struct IndexBuffer {
    public:
        IndexBuffer(const void * data, size_t size, unsigned int usage);
        ~IndexBuffer();

        IndexBuffer(const IndexBuffer &) = delete;
        IndexBuffer& operator=(const IndexBuffer &) = delete;
        IndexBuffer(IndexBuffer&&) noexcept;
        IndexBuffer& operator=(IndexBuffer&&) noexcept;

        void Reset(const void * data, size_t size);

        void bind();
        void unbind();

        [[nodiscard]] inline unsigned int GetId() const {
            return ID;
        }
        [[nodiscard]] inline unsigned int GetSize() const {
            return m_numbers;
        }
    private:
        unsigned int ID;
        size_t m_numbers;
    };
}


#endif //GAMEENGINE_INDEXBUFFER_H
