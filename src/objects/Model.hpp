#include <GL/glew.h>
#include <initializer_list>
#include <string>

#pragma once

class Model {
protected:
    GLuint VAO;
    GLuint VBO;
    void load_model(const std::initializer_list<float> &vertices) const;
    void bind_buffer(const std::initializer_list<float> &vertices) const;
    void vertices_normal() const;
    void vertices_uv() const;

public:
    void unbind() const;
    virtual void render() = 0;
    Model();
    ~Model();
};

namespace models {

struct Vertex {
    float position[3];
    float normal[3];
    float texture[2];
    float tangent[3];
};

class ObjectFile : public Model {
private:
    int indices_count;

public:
    ObjectFile(std::string file_name);
    void render() override;
};

class Triangle : public Model {
public:
    void render() override;
    Triangle();
};

class Cube : public Model {
public:
    void render() override;
    Cube();
};

class Tree : public Model {
public:
    void render() override;
    Tree();
};

class Bush : public Model {
public:
    void render() override;
    Bush();
};

class Sphere : public Model {
public:
    void render() override;
    Sphere();
};

class Plain : public Model {
public:
    void render() override;
    Plain();
};

class SkyCube : public Model {
public:
    void render() override;
    SkyCube();
};
} // namespace models
