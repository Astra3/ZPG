#pragma once
#include <cstddef>
class Camera;
class Light;
namespace lights {
class Point;
class Directional;
} // namespace lights

class Observer {
public:
    virtual void update(Camera &camera) = 0;
    virtual void update(Light &light, size_t light_id = 0) = 0;
};
