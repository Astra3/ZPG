#include "../observers/Observer.hpp"
#include <GL/glew.h>
#include <fstream>
#include <glm/ext/matrix_float3x3.hpp>

#pragma once

class ShaderProgram : public Observer {
private:
    void initialize(const char *vertex_source, const char *fragment_source);
    GLuint shader_program_id;

public:
    ShaderProgram(const char *vertex_source, const char *fragment_source);
    ShaderProgram(const std::ifstream &vertex_file, const std::ifstream &fragment_file);
    void apply_transformation(std::string name, const glm::mat4 &mat) const;
    void apply_transformation(std::string name, const glm::vec3 &vec) const;
    void apply_transformation(std::string name, const float value) const;
    void use() const;
    void unuse() const;
    ~ShaderProgram();
    void update(Camera &camera) override;
    void update(lights::PositionedLight &light, size_t light_id = 0) override;
};
