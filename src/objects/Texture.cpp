#include "Texture.hpp"
#include "SOIL2.h"
#include <cstdlib>
#include <iostream>

Texture::Texture(std::string filename) {
    glActiveTexture(GL_TEXTURE0);
    this->texture_id = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (this->texture_id == 0) {
        std::cerr << "Failed to load texture: " << SOIL_last_result() << std::endl;
        exit(EXIT_FAILURE);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture::bind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
}

void Texture::apply_uniform(ShaderProgram &shader_program) { shader_program.apply_transformation("tex_unit_id", 0); }

CubeMap::CubeMap(std::string x_pos, std::string x_neg, std::string y_pos, std::string y_neg, std::string z_pos,
                 std::string z_neg) {
    glActiveTexture(GL_TEXTURE0);
    this->texture_id = SOIL_load_OGL_cubemap(x_pos.c_str(), x_neg.c_str(), y_pos.c_str(), y_neg.c_str(), z_pos.c_str(),
                                             z_neg.c_str(), SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
}

void CubeMap::bind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture_id);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CubeMap::apply_uniform(ShaderProgram &shader_program) { shader_program.apply_transformation("cubemap", 0); }
