#include "VAO.hpp"
#include "VBO.hpp"

VAO::VAO() {
    glGenVertexArrays(1, &this->vao_id);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &this->vao_id);
}

void VAO::bind() {
    glBindVertexArray(this->vao_id);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

void VAO::link_vbo(VBO &vbo, GLuint index) {
    vbo.bind();
    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(index);
    vbo.unbind();
}

