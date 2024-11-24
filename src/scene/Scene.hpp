#pragma once
#include "../generators.hpp"
#include "../objects/DrawableObject.hpp"
#include "../objects/Skybox.hpp"
#include <memory>
#include <optional>
#include <vector>

class Scene {
private:
    std::vector<DrawableObject> objects;
    std::optional<SkyBox> skybox = std::nullopt;
    void initialize_lights();

public:
    std::optional<std::vector<std::unique_ptr<Light>>> lights;
    Scene() {}
    Scene(std::unique_ptr<Light> light);
    Scene(std::unique_ptr<Light> light, std::shared_ptr<Camera> camera);
    void add_model(DrawableObject object);
    void add_light(std::unique_ptr<Light> light);
    void reset_light_ids();
    void apply_generator(generators::GENERATOR_FUNCTION generator, std::shared_ptr<ShaderProgram> shader, size_t count);
    void render();
};
