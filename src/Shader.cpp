#include "Shader.hpp"
#include <iostream>

// TODO make this raise exception on failue and also maybe make it part of the class
void check_shader(GLuint shader) {
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        std::cerr << "Shader compilation failed: " << info_log << std::endl;
        exit(EXIT_FAILURE);
    }
}

Shader::Shader(const char* vertex_shader_source, const char* fragment_shader_source) {
    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    check_shader(vertex_shader);

    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    check_shader(fragment_shader);

    this->shader_program_id = glCreateProgram();
    glAttachShader(this->shader_program_id, vertex_shader);
    glAttachShader(this->shader_program_id, fragment_shader);
    glLinkProgram(this->shader_program_id);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::~Shader() {
    glDeleteProgram(this->shader_program_id);
}

void Shader::use() {
    glUseProgram(this->shader_program_id);
}

void Shader::unuse() {
    glUseProgram(0);
}

