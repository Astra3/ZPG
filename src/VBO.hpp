#include <initializer_list>
#include <GL/glew.h>

#pragma once

class VBO {
private:
    GLuint VBO_id;
    std::initializer_list<float> points;
public:
    VBO(std::initializer_list<float> points);
    void bind();
    void unbind();
    ~VBO();
};

