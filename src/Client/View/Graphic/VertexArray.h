#ifndef GAMEENGINE_VERTEXARRAY_H
#define GAMEENGINE_VERTEXARRAY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Graphic {
    struct VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray &) = delete;
        VertexArray& operator=(const VertexArray &) = delete;
        VertexArray(VertexArray&&) noexcept;
        VertexArray& operator=(VertexArray&&) noexcept;

        size_t SetVertexAttrib(size_t index = 0,
                               size_t size = 3,
                               size_t stride = 0 * sizeof(float),
                               unsigned int type = GL_FLOAT,
                               bool normalized = GL_FALSE,
                               const void * offset = nullptr);

        void bind();
        void unbind();

        void EnableAllAttrib();
        void DisableAllAttrib();

        bool EnableAttrib(size_t i);
        bool DisableAttrib(size_t i);

        [[nodiscard]] inline unsigned int GetId() const {
            return ID;
        }
        [[nodiscard]] inline size_t GetLastAttrib() const {
            return m_last_attrib;
        }

    private:
        unsigned int ID;
        size_t m_last_attrib;
    };

}

#endif //GAMEENGINE_VERTEXARRAY_H
