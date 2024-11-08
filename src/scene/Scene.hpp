#pragma once
#include "../generators.hpp"
#include "../objects/DrawableObject.hpp"
#include <memory>
#include <vector>

class Scene {
private:
    std::vector<DrawableObject> objects;
    void initialize_lights();

public:
    std::optional<std::vector<std::unique_ptr<Light>>> lights;
    Scene() {}
    Scene(std::unique_ptr<Light> light);
    void add_model(DrawableObject object);
    void add_light(std::unique_ptr<Light> light);
    void reset_light_ids();
    void apply_generator(generators::GENERATOR_FUNCTION generator, std::shared_ptr<ShaderProgram> shader, size_t count);
    void render();
};
