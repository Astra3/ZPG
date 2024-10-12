#include "VBO.hpp"
#include <GL/glew.h>

#pragma once

class VAO {
private:
    GLuint vao_id;
public:
    VAO();
    ~VAO();
    void bind();
    void unbind();
    void link_vbo(VBO &vbo, GLuint layout);
};

