#pragma once
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/trigonometric.hpp>
#include "Observable.hpp"

class Camera : public Observable {
private:
    glm::mat4 view;
    glm::mat4 projection;
public:
    Camera();
    glm::mat4 get_projection();
    glm::mat4 get_view();
    void notify_observers() override;
};
