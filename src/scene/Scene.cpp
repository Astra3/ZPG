#include "Scene.hpp"
#include "../scene/Light.hpp"
#include <algorithm>
#include <memory>
#include <optional>

void Scene::add_model(DrawableObject object) { objects.push_back(std::move(object)); }

void Scene::initialize_lights() { this->lights = std::make_optional<std::vector<std::unique_ptr<Light>>>(); }

Scene::Scene(std::unique_ptr<Light> light) {
    this->initialize_lights();
    light->set_id(0);
    this->lights->push_back(std::move(light));
}

Scene::Scene(std::unique_ptr<Light> light, std::shared_ptr<Camera> camera) : skybox(camera) {
    this->initialize_lights();
    light->set_id(0);
    this->lights->push_back(std::move(light));
}

void Scene::apply_generator(generators::GENERATOR_FUNCTION generator, std::shared_ptr<ShaderProgram> shader,
                            size_t count) {
    generator(this->objects, shader, count);
}

void Scene::add_light(std::unique_ptr<Light> light) {
    if (!this->lights.has_value())
        this->initialize_lights();
    light->set_id(this->lights->size() == 0 ? 0 : this->lights->size());
    this->lights->push_back(std::move(light));
}

void Scene::reset_light_ids() {
    if (this->lights.has_value()) {
        for (size_t i = 0; i < this->lights->size(); i++) {
            (*this->lights)[i]->set_id(i);
        }
    }
}

void Scene::render() {
    if (this->lights.has_value()) {
        for (auto &light : *this->lights) {
            light->notify_observers();
        }
    }
    if (this->skybox.has_value()) {
        this->skybox->render();
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    std::ranges::for_each(this->objects, &DrawableObject::render);
}
