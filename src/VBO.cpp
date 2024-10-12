#include "VBO.hpp"
#include <initializer_list>
#include <iostream>
#include <iterator>

float tr_points[] = {
    0.0f,  0.8f, 0.0f,
    0.5f, -0.5f, 0.0f,
   -0.3f, -0.5f, 0.0f
};

VBO::VBO(std::initializer_list<float> points) : points(points) {
    glGenBuffers(1, &this->VBO_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO_id);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.begin(), GL_STATIC_DRAW);
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO_id);
    std::cout << "data: " << std::data(points)[1] << std::endl;
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VBO::~VBO() {
    glDeleteBuffers(1, &this->VBO_id);
}

