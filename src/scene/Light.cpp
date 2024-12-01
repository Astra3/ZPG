#include "Light.hpp"
#include "../Camera.hpp"

void Light::set_id(size_t id) { this->id = id; }

void Light::notify_observers() {
    for (auto &observer : this->observers) {
        observer->update(*this, this->id);
    }
}

namespace lights {
void Point::set_position(glm::vec3 position) {
    this->position = position;
    this->notify_observers();
}

glm::vec3 Point::get_position() const { return this->position; }

glm::vec3 Directional::get_direction() const { return this->direction; }

glm::vec3 Spot::get_direction() const { return this->direction; }
glm::vec3 Spot::get_position() const { return this->position; }
float Spot::get_cut_off() const { return this->cut_off; }
float Spot::get_outer_cut_off() const { return this->outer_cut_off; }

glm::vec3 Flashlight::get_direction() const { return this->camera->get_front(); }

glm::vec3 Flashlight::get_position() const { return this->camera->get_position(); }

} // namespace lights
