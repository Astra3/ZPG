#include "Shader.hpp"
#include "Camera.hpp"
#include <fstream>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <sstream>

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

std::string read_file(const std::ifstream &file) {
    std::stringstream contents;
    contents << file.rdbuf();
    return contents.str();
}

void Shader::initialize(const char *vertex_source, const char *fragment_source) {
    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);
    check_shader(vertex_shader);

    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);
    check_shader(fragment_shader);

    this->shader_program_id = glCreateProgram();
    glAttachShader(this->shader_program_id, vertex_shader);
    glAttachShader(this->shader_program_id, fragment_shader);
    glLinkProgram(this->shader_program_id);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::Shader(const std::ifstream vertex_file, const std::ifstream fragment_file) {
    auto vertex_shader = read_file(vertex_file);
    auto fragment_shader = read_file(fragment_file);
    this->initialize(vertex_shader.c_str(), fragment_shader.c_str());
}

Shader::Shader(const char *vertex_source, const char *fragment_source) {
    this->initialize(vertex_source, fragment_source);
}

void Shader::apply_transformation(const char *name, const glm::mat4 &mat) const {
    uint transform_loc = glGetUniformLocation(this->shader_program_id, name);
    glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::update(Camera &camera) {
    this->apply_transformation("view", camera.get_view());
    this->apply_transformation("projection", camera.get_projection());
}

Shader::~Shader() { glDeleteProgram(this->shader_program_id); }

void Shader::use() const { glUseProgram(this->shader_program_id); }

void Shader::unuse() const { glUseProgram(0); }
