#include <GL/glew.h>

#pragma once

class Model {
protected:
    GLuint VAO;
    GLuint VBO;

public:
    void unbind() const;
    virtual void render() = 0;
    Model();
    ~Model();
};

namespace models {

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
} // namespace models
