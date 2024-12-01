#pragma once
#include "ShaderProgram.hpp"
#include <glm/ext/vector_float3.hpp>
class Material {
public:
    glm::vec3 ambient{1.0f};
    glm::vec3 diffuse{1.0f};
    glm::vec3 specular{1.0f};
    float shininess = 32.0f;
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
        : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}
    Material() {}
    void apply(ShaderProgram &shader_program);
};
