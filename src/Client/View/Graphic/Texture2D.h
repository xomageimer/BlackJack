#ifndef GAMEENGINE_TEXTURE2D_H
#define GAMEENGINE_TEXTURE2D_H

#include <vector>
#include <filesystem>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

// TODO это класс конфигураций текстур (вершины будут задаваться в классах объектов, кт используют текстуры (а в спрайтах они будут изменяться)

namespace Graphic {

    struct Texture2D {
    public:
        explicit Texture2D(const std::filesystem::path &, unsigned int type = GL_RGBA, unsigned int format = GL_RGBA, bool flip_vertically = true);

        Texture2D(const Texture2D &) = delete;
        Texture2D &operator=(const Texture2D &) = delete;
        Texture2D(Texture2D &&) noexcept ;
        Texture2D &operator=(Texture2D &&) noexcept ;

        void SetFilter(unsigned int type_filter = GL_TEXTURE_MAG_FILTER, unsigned int new_filter = GL_LINEAR);
        void SetMode(unsigned int wrap_coordinate, unsigned int new_mode = GL_CLAMP_TO_BORDER, const std::vector<float> & color = defaultcolor);

        void FreeTrash();
        [[deprecated]] void GenerateMipmap();

        void bind();
        void unbind();

        [[nodiscard]] inline unsigned int GetHeight() const {
            return m_height;
        }
        [[nodiscard]] inline unsigned int GetWidth() const {
            return m_width;
        }

        ~Texture2D();

    private:
        unsigned int ID;

        int m_width;
        int m_height;
        int m_nrChannels;
        unsigned char * m_data;

        const inline static std::vector<float> defaultcolor = {1.0f, 1.0f, 1.0f, 1.0f};
    };

}

#endif //GAMEENGINE_TEXTURE2D_H
