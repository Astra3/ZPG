#include "Model.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "Transformation.hpp"
#include <GL/glew.h>
#include <memory>
#include <optional>
#include <vector>

#pragma once

using TransformationType = std::vector<std::shared_ptr<Transformation>>;

class DrawableObject {
private:
    std::shared_ptr<Model> model;
    std::shared_ptr<ShaderProgram> shader;
    std::optional<TransformationType> transformations = std::nullopt;
    std::optional<std::shared_ptr<Texturable>> texture = std::nullopt;

public:
    DrawableObject(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> shader)
        : model(model), shader(shader) {}
    DrawableObject(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> shader,
                   TransformationType transformations)
        : model(model), shader(shader), transformations(std::move(transformations)) {}
    DrawableObject(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> shader,
                   TransformationType transformations, std::shared_ptr<Texturable> texture)
        : model(model), shader(shader), transformations(std::move(transformations)), texture(texture) {}
    void render();
};
