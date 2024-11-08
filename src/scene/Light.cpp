#include "Light.hpp"

void Light::set_color(glm::vec3 color) {
    this->color = color;
    this->notify_observers();
}

void Light::set_id(size_t id) { this->id = id; }

namespace lights {
void PositionedLight::notify_observers() {
    for (auto &observer : this->observers) {
        observer->update(*this, this->id);
    }
}
void PositionedLight::set_position(glm::vec3 position) {
    this->position = position;
    this->notify_observers();
}

LightData PositionedLight::get_data() {
    return LightData{.color = this->color,
                     .position = this->position,
                     .constant = this->constant,
                     .linear = this->linear,
                     .quadratic = this->quadratic};
}
} // namespace lights
