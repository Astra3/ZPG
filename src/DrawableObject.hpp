#include "Model.hpp"
#include "Shader.hpp"
#include "Transformation.hpp"
#include <GL/glew.h>
#include <memory>
#include <optional>
#include <vector>

#pragma once

using TransformationType = std::vector<std::unique_ptr<Transformation>>;

class DrawableObject {
private:
    std::shared_ptr<Model> model;
    std::shared_ptr<Shader> shader;
    std::optional<TransformationType> transformations = std::nullopt;

public:
    DrawableObject(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader) : model(model), shader(shader) {}
    DrawableObject(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, TransformationType transformations)
        : model(model), shader(shader), transformations(std::move(transformations)) {}
    void render();
};