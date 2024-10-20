#pragma once
class Camera;

class Observer {
public:
    virtual void update(Camera &camera) = 0;
};
