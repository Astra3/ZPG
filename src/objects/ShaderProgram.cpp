#include "ShaderProgram.hpp"
#include "../Camera.hpp"
#include "../scene/Light.hpp"
#include <cstdlib>
#include <fstream>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

// TODO make this raise exception on failue and also maybe make it part of the class
void check_shader(GLuint shader) {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        auto str_info = std::make_unique<char[]>(length + 1);
        glGetShaderInfoLog(shader, length, NULL, str_info.get());
        std::cerr << "Shader compilation failed: " << str_info << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::string read_file(const std::ifstream &file) {
    std::stringstream contents;
    contents << file.rdbuf();
    return contents.str();
}

void ShaderProgram::initialize(const char *vertex_source, const char *fragment_source) {
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
    int success;
    glGetProgramiv(this->shader_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        int length;
        glGetProgramiv(this->shader_program_id, GL_INFO_LOG_LENGTH, &length);
        auto str_info = std::make_unique<char[]>(length + 1);
        glGetProgramInfoLog(this->shader_program_id, length, NULL, str_info.get());
        std::cerr << "Shader program linking failed: " << str_info << std::endl;
        exit(EXIT_FAILURE);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

ShaderProgram::ShaderProgram(const std::ifstream &vertex_file, const std::ifstream &fragment_file) {
    auto vertex_shader = read_file(vertex_file);
    auto fragment_shader = read_file(fragment_file);
    this->initialize(vertex_shader.c_str(), fragment_shader.c_str());
}

ShaderProgram::ShaderProgram(const char *vertex_source, const char *fragment_source) {
    this->initialize(vertex_source, fragment_source);
}

void ShaderProgram::apply_transformation(std::string name, const glm::mat4 &mat) const {
    uint transform_loc = glGetUniformLocation(this->shader_program_id, name.c_str());
    glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::apply_transformation(std::string name, const glm::vec3 &vec) const {
    uint transform_loc = glGetUniformLocation(this->shader_program_id, name.c_str());
    glUniform3fv(transform_loc, 1, glm::value_ptr(vec));
}
void ShaderProgram::apply_transformation(std::string name, const float value) const {
    uint transform_lot = glGetUniformLocation(this->shader_program_id, name.c_str());
    glUniform1f(transform_lot, value);
}

void ShaderProgram::update(Camera &camera) {
    this->use();
    this->apply_transformation("view_pos", camera.get_position());
    this->apply_transformation("view", camera.get_view());
    this->apply_transformation("projection", camera.get_projection());
}

void ShaderProgram::update(lights::PositionedLight &light, size_t light_id) {
    this->use();
    auto data = light.get_data();
    std::string struct_name = "point_lights[" + std::to_string(light_id) + "].";
    this->apply_transformation(struct_name + "color", data.color);
    this->apply_transformation(struct_name + "position", data.position);

    this->apply_transformation(struct_name + "constant", data.constant);
    this->apply_transformation(struct_name + "linear", data.linear);
    this->apply_transformation(struct_name + "quadratic", data.quadratic);
}

ShaderProgram::~ShaderProgram() { glDeleteProgram(this->shader_program_id); }

void ShaderProgram::use() const { glUseProgram(this->shader_program_id); }

void ShaderProgram::unuse() const { glUseProgram(0); }
