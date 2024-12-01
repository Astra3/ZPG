#include "../observers/Observable.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#pragma once

struct LightStrength {
    glm::vec3 ambient{0.1f};
    glm::vec3 specular{0.8f};
    glm::vec3 diffuse{10.f};
};

class Light : public Observable {
public:
    LightStrength light_strength;

protected:
    size_t id = 0;

public:
    Light(size_t id = 0) : id(id) {}
    void set_id(size_t id);
    void notify_observers() override;
};

struct AttenuationData {
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.32f;
};

class AttenuatedLight : public Light {
public:
    AttenuationData attenuation;
    AttenuatedLight(size_t id = 0) : Light(id) {}
};

namespace lights {

class Point : public AttenuatedLight {
private:
    glm::vec3 position{0.0f};

public:
    Point(glm::vec3 position, size_t id = 0) : AttenuatedLight(id), position(position) {}
    void set_position(glm::vec3 position);
    glm::vec3 get_position() const;
};

class Directional : public Light {
private:
    glm::vec3 direction{0.0f};

public:
    Directional(glm::vec3 direction, size_t id = 0) : Light(id), direction(direction) {}
    glm::vec3 get_direction() const;
};

class Spot : public AttenuatedLight {
private:
    glm::vec3 position{0.0f};
    glm::vec3 direction{0.0f};
    float cut_off = glm::cos(glm::radians(12.5f));
    float outer_cut_off = glm::cos(glm::radians(20.f));

public:
    Spot(glm::vec3 direction, glm::vec3 position, size_t id = 0)
        : AttenuatedLight(id), direction(direction), position(position) {}
    Spot() {};
    glm::vec3 get_direction() const;
    glm::vec3 get_position() const;
    float get_cut_off() const;
    float get_outer_cut_off() const;
};

class Flashlight : public Spot {
private:
    std::shared_ptr<Camera> camera;

public:
    Flashlight(std::shared_ptr<Camera> camera) : camera(camera) {}
    glm::vec3 get_direction() const;
    glm::vec3 get_position() const;
};
} // namespace lights
