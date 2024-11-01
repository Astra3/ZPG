#pragma once
#include "../generators.hpp"
#include "../objects/DrawableObject.hpp"
#include <functional>
#include <memory>
#include <vector>

class Scene {
private:
    std::vector<DrawableObject> objects;
    std::optional<std::shared_ptr<Light>> light;

public:
    Scene() {}
    Scene(std::shared_ptr<Light> light) : light(light) {}
    void add_model(DrawableObject object);
    void apply_generator(generators::GENERATOR_FUNCTION generator, std::shared_ptr<ShaderProgram> camera, size_t count);
    void render();
};
