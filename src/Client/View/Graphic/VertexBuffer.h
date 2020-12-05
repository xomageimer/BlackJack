#ifndef GAMEENGINE_VERTEXBUFFER_H
#define GAMEENGINE_VERTEXBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Graphic {

    struct VertexBuffer {
    public:
        VertexBuffer(const void * data, size_t size, unsigned int usage);

        VertexBuffer(const VertexBuffer &) = delete;
        VertexBuffer& operator=(const VertexBuffer &) = delete;

        VertexBuffer(VertexBuffer&&) noexcept;
        VertexBuffer& operator=(VertexBuffer &&) noexcept;

        void Reset(const void * data, size_t size);

        void bind();
        void unbind();

        ~VertexBuffer();

        [[nodiscard]] inline unsigned int GetId() const {
            return ID;
        }

    private:
        unsigned int ID{};
    };

}


#endif //GAMEENGINE_VERTEXBUFFER_H
