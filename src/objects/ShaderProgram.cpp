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
    this->use();
    uint transform_loc = glGetUniformLocation(this->shader_program_id, name.c_str());
    glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(mat));
    glUseProgram(0);
}

void ShaderProgram::apply_transformation(std::string name, const glm::vec3 &vec) const {
    this->use();
    uint transform_loc = glGetUniformLocation(this->shader_program_id, name.c_str());
    glUniform3fv(transform_loc, 1, glm::value_ptr(vec));
    glUseProgram(0);
}
void ShaderProgram::apply_transformation(std::string name, const float value) const {
    this->use();
    uint transform_lot = glGetUniformLocation(this->shader_program_id, name.c_str());
    glUniform1f(transform_lot, value);
    glUseProgram(0);
}

void ShaderProgram::apply_transformation(std::string name, const int value) const {
    this->use();
    uint transform_lot = glGetUniformLocation(this->shader_program_id, name.c_str());
    glUniform1i(transform_lot, value);
    glUseProgram(0);
}

void ShaderProgram::apply_transformation(std::string name, const uint value) const {
    this->use();
    auto transform_lot = glGetUniformLocation(this->shader_program_id, name.c_str());
    glUniform1ui(transform_lot, value);
    glUseProgram(0);
}

void ShaderProgram::apply_transformation(std::string name, const bool value) const {
    this->use();
    auto transform_lot = glGetUniformLocation(this->shader_program_id, name.c_str());
    glUniform1i(transform_lot, value ? 1 : 0);
    glUseProgram(0);
}

void ShaderProgram::update(Camera &camera) {
    this->apply_transformation("view_pos", camera.get_position());
    this->apply_transformation("view", camera.get_view());
    this->apply_transformation("projection", camera.get_projection());
}

void ShaderProgram::apply_attenuation(std::string &struct_name, AttenuationData attenuation) {
    this->apply_transformation(struct_name + "constant", attenuation.constant);
    this->apply_transformation(struct_name + "linear", attenuation.linear);
    this->apply_transformation(struct_name + "quadratic", attenuation.quadratic);
}

void ShaderProgram::apply_light_strength(std::string &struct_name, LightStrength strength) {
    this->apply_transformation(struct_name + "ambient", strength.ambient);
    this->apply_transformation(struct_name + "specular", strength.specular);
    this->apply_transformation(struct_name + "diffuse", strength.diffuse);
}

void ShaderProgram::update(Light &light, size_t light_id) {
    auto positional = dynamic_cast<lights::Point *>(&light);
    if (positional != nullptr) {
        std::string struct_name = "point_lights[" + std::to_string(light_id) + "].";
        this->apply_transformation(struct_name + "position", positional->get_position());

        this->apply_light_strength(struct_name, positional->light_strength);
        this->apply_attenuation(struct_name, positional->attenuation);

        this->point_light_count++;
        this->apply_transformation("point_light_count", this->point_light_count);
        return;
    }

    auto directional = dynamic_cast<lights::Directional *>(&light);
    if (directional != nullptr) {
        std::string struct_name = "directional_lights[" + std::to_string(light_id) + "].";
        this->apply_transformation(struct_name + "direction", directional->get_direction());

        this->apply_light_strength(struct_name, directional->light_strength);
        this->dir_light_count++;
        this->apply_transformation("dir_light_count", this->dir_light_count);
        return;
    }

    auto spot = dynamic_cast<lights::Flashlight *>(&light);
    if (spot != nullptr) {
        std::string struct_name = "spot_lights[" + std::to_string(light_id) + "].";
        this->apply_transformation(struct_name + "direction", spot->get_direction());
        this->apply_transformation(struct_name + "position", spot->get_position());
        this->apply_transformation(struct_name + "cut_off", spot->get_cut_off());
        this->apply_transformation(struct_name + "outer_cut_off", spot->get_outer_cut_off());

        this->apply_light_strength(struct_name, spot->light_strength);
        this->apply_attenuation(struct_name, spot->attenuation);

        this->spot_light_count++;
        this->apply_transformation("spot_light_count", this->dir_light_count);
        return;
    }
}

ShaderProgram::~ShaderProgram() { glDeleteProgram(this->shader_program_id); }

void ShaderProgram::render() {
    this->use();
    this->point_light_count = 0;
    this->spot_light_count = 0;
    this->dir_light_count = 0;
}

void ShaderProgram::use() const { glUseProgram(this->shader_program_id); }

void ShaderProgram::unuse() const { glUseProgram(0); }

std::shared_ptr<ShaderProgram> ShaderProgram::create_texture(Camera &camera) {
    auto shader = std::make_shared<ShaderProgram>(std::ifstream("../src/shaders/texture_vertex.glsl"),
                                                  std::ifstream("../src/shaders/texture_fragment.glsl"));
    camera.attach(shader);
    return shader;
}

std::shared_ptr<ShaderProgram> ShaderProgram::create_normal_blinn(Camera &camera) {
    auto shader = std::make_shared<ShaderProgram>(std::ifstream("../src/shaders/general_vertex.glsl"),
                                                  std::ifstream("../src/shaders/blinn_fragment.glsl"));
    camera.attach(shader);
    return shader;
}
