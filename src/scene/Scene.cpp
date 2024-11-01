#include "Scene.hpp"
#include "../scene/Light.hpp"
#include <algorithm>

void Scene::add_model(DrawableObject object) { objects.push_back(std::move(object)); }

void Scene::apply_generator(generators::GENERATOR_FUNCTION generator, std::shared_ptr<ShaderProgram> camera,
                            size_t count) {
    generator(this->objects, camera, count);
}

void Scene::render() {
    if (this->light.has_value()) {
        (*this->light)->notify_observers();
    }
    std::ranges::for_each(this->objects, &DrawableObject::render);
}
