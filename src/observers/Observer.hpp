#pragma once
class Camera;
class Light;
namespace lights {
class PositionedLight;
}

class Observer {
public:
    virtual void update(Camera &camera) = 0;
    virtual void update(lights::PositionedLight &light) = 0;
};
