#include "Material.hpp"
#include "ShaderProgram.hpp"

void Material::apply(ShaderProgram &shader_program) {
    shader_program.apply_transformation("material.ambient", this->ambient);
    shader_program.apply_transformation("material.diffuse", this->diffuse);
    shader_program.apply_transformation("material.specular", this->specular);
    shader_program.apply_transformation("material.shininess", this->shininess);
}
