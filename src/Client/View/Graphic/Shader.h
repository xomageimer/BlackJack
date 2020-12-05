#ifndef GAMEENGINE_SHADER_H
#define GAMEENGINE_SHADER_H

#include <filesystem>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define UNIFORMTAG(ph_1, ph_2) glUniform ## ph_1 ## ph_2

#define CallFuncs(type, separate) CallIfCallable(UNIFORMTAG(1, type), glGetUniformLocation(ID, val_name.c_str()), std::forward<Params>(params)...) separate \
    CallIfCallable(UNIFORMTAG(2, type), glGetUniformLocation(ID, val_name.c_str()), std::forward<Params>(params)...) separate \
    CallIfCallable(UNIFORMTAG(3, type), glGetUniformLocation(ID, val_name.c_str()), std::forward<Params>(params)...) separate  \
    CallIfCallable(UNIFORMTAG(4, type), glGetUniformLocation(ID, val_name.c_str()), std::forward<Params>(params)...)

#define CallFuncsForMatrix(type, separate) CallIfCallable(UNIFORMTAG(type, 2fv), glGetUniformLocation(ID, val_name.c_str()), 1, GL_FALSE, glm::value_ptr(std::forward<Params>(params)...)) separate \
    CallIfCallable(UNIFORMTAG(type, 3fv), glGetUniformLocation(ID, val_name.c_str()), 1, GL_FALSE, glm::value_ptr(std::forward<Params>(params)...)) separate  \
    CallIfCallable(UNIFORMTAG(type, 4fv), glGetUniformLocation(ID, val_name.c_str()), 1, GL_FALSE, glm::value_ptr(std::forward<Params>(params)...))


template <typename F, typename... S>
constexpr void CallIfCallable(F&& func, S&& ...args){
    if constexpr (std::is_invocable_v<F, decltype(std::forward<S>(args))...>){
        std::invoke(std::forward<F>(func), std::forward<S>(args)...);
    } else {
        return;
    }
};

struct Shader {
public:
    Shader(const std::filesystem::path & vertex_shader, const std::filesystem::path & fragment_shader);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&&) noexcept;
    Shader& operator=(Shader&&) noexcept;

    void use();

    template <typename ... Params>
    void glUniform(const std::string &val_name, Params... params){
        if constexpr (std::is_same_v<Params..., float>) {
            CallFuncs(f, ;);
        }
        else if constexpr (std::is_same_v<Params..., int>) {
            CallFuncs(i, ;);
        }
        else if constexpr (std::is_same_v<Params..., glm::mat4>){
            CallFuncsForMatrix(Matrix, ;);
        }
    }

    bool Check(unsigned int &);

    [[nodiscard]] unsigned int GetId() const {
        return ID;
    }
private:
    unsigned int ID;
};


#endif //GAMEENGINE_SHADER_H
