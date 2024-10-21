#include "Observer.hpp"
#include <GL/glew.h>
#include <fstream>
#include <glm/ext/matrix_float3x3.hpp>

#pragma once

class Shader : public Observer {
private:
    void initialize(const char *vertex_source, const char *fragment_source);
    GLuint shader_program_id;

public:
    Shader(const char *vertex_source, const char *fragment_source);
    Shader(const std::ifstream vertex_file, const std::ifstream fragment_file);
    void apply_transformation(const char *name, const glm::mat4 &mat) const;
    void use() const;
    void unuse() const;
    ~Shader();
    void update(Camera &camera) override;
};
