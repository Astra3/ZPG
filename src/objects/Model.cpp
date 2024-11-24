#include "Model.hpp"
#include "../models/bushes.hpp"
#include "../models/cube.hpp"
#include "../models/plain.hpp"
#include "../models/skycube.hpp"
#include "../models/sphere.hpp"
#include "../models/tree.hpp"
#include <GL/gl.h>
#include <GL/glext.h>
#include <initializer_list>

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

void Model::load_model(const std::initializer_list<float> &vertices) const {
    this->bind_buffer(vertices);
    this->vertices_normal();
}

void Model::bind_buffer(const std::initializer_list<float> &vertices) const {
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.begin(), GL_STATIC_DRAW);
}

void Model::vertices_normal() const {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)(0 * sizeof(float)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)(3 * sizeof(float)));
}

void Model::vertices_uv() const {
    glEnableVertexAttribArray(0); // enable vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid *)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid *)(6 * sizeof(float)));
}

namespace models {
auto triangle_points = {
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
};
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
}

void Cube::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

Tree::Tree() : Model() { this->load_model(tree_vertices); }

void Tree::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 92814);
}

Bush::Bush() : Model() { this->load_model(bushes_vertices); }

void Bush::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 8730);
}

Sphere::Sphere() : Model() { this->load_model(sphere_vertices); }

void Sphere::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 2880);
}

Plain::Plain() : Model() {
    this->bind_buffer(plain_vertices);
    this->vertices_uv();
}

void Plain::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

SkyCube::SkyCube() {
    this->bind_buffer(skycube_vertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
}

void SkyCube::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 108);
}

} // namespace models
