#include "Shader.h"

Shader::Shader(const std::filesystem::path &vertex_shader,
               const std::filesystem::path &fragment_shader) {
    std::fstream vertex_shader_file;
    std::fstream fragment_shader_file;

    vertex_shader_file.exceptions(std::fstream::badbit | std::fstream::failbit);
    fragment_shader_file.exceptions(std::fstream::badbit | std::fstream::failbit);

    std::string vertex_shader_str;
    std::string fragment_shader_str;

    try {
        std::stringstream vertex_shader_stream;
        std::stringstream fragment_shader_stream;

        vertex_shader_file.open(vertex_shader);
        fragment_shader_file.open(fragment_shader);

        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();

        vertex_shader_str = vertex_shader_stream.str();
        fragment_shader_str = fragment_shader_stream.str();

        vertex_shader_file.close();
        fragment_shader_file.close();

    } catch(std::ifstream::failure & except){
        std::cerr << "Vertex shader path: " << vertex_shader << std::endl;
        std::cerr << "Fragment shader path: " << fragment_shader << std::endl;
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << except.what() << std::endl;
    }

    unsigned int vertex_shader_id, fragment_shader_id;
    const char * vertex_shader_code = vertex_shader_str.c_str();
    const char * fragment_shader_code = fragment_shader_str.c_str();

    vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex_shader_id, 1, &vertex_shader_code, nullptr);
    glShaderSource(fragment_shader_id, 1, &fragment_shader_code, nullptr);

    glCompileShader(vertex_shader_id);
    glCompileShader(fragment_shader_id);

    Check(vertex_shader_id);
    Check(fragment_shader_id);

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex_shader_id);
    glAttachShader(this->ID, fragment_shader_id);

    glLinkProgram(this->ID);

    int success;
    char infolog[512];
    glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(this->ID, 512, nullptr, infolog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infolog << std::endl;
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}

bool Shader::Check(unsigned int & shader_id) {
    int success;
    char infolog[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(shader_id, 512, nullptr, infolog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infolog << std::endl;
        return false;
    }
    return true;
}

Shader::~Shader() {
    glDeleteProgram(this->ID);
}

Shader::Shader(Shader && other) noexcept {
    this->ID = other.ID;
    other.ID = 0;
}

Shader &Shader::operator=(Shader && other) noexcept {
    this->ID = other.ID;
    other.ID = 0;

    return *this;
}

void Shader::use() {
    glUseProgram(this->ID);
}
