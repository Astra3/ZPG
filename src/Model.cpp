#include "Model.hpp"
#include "models/cube.hpp"
#include "models/tree.hpp"
#include "models/bushes.hpp"
#include <GL/gl.h>
#include <GL/glext.h>
#include <initializer_list>
#include <iostream>
#include <ostream>

void Model::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Model::Model() {
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
}

Model::~Model() {
    glDeleteBuffers(1, &this->VBO);
    glDeleteVertexArrays(1, &this->VAO);
}

namespace models {
auto triangle_points = {0.0f, 0.8f, 0.0f, 0.5f, -0.5f, 0.0f, -0.3f, -0.5f, 0.0f};
Triangle::Triangle() : Model() {
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, triangle_points.size() * sizeof(float), triangle_points.begin(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void Triangle::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

Cube::Cube() : Model() {
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, cube_vertices.size() * sizeof(float), cube_vertices.begin(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
    std::cout << "size: " << cube_vertices.size() << std::endl;
}

void Cube::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

Tree::Tree() : Model() {
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, tree_vertices.size() * sizeof(float), tree_vertices.begin(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(0 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
}

void Tree::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 92814);
}

Bush::Bush() : Model() {
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, bushes_vertices.size() * sizeof(float), bushes_vertices.begin(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(0 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
}

void Bush::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 8730);
}

const std::initializer_list<float> sphere_points = {-.5f, -.5f, .5f, 0, 0, 1, -.5f, .5f,  .5f, 0, 0, 1,
                                                    .5f,  .5f,  .5f, 0, 0, 1, .5f,  -.5f, .5f, 0, 0, 1};

Sphere::Sphere() : Model() {
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sphere_points.size() * sizeof(float), sphere_points.begin(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)(3 * sizeof(float)));
}

void Sphere::render() { glBindVertexArray(this->VAO); }

} // namespace models
