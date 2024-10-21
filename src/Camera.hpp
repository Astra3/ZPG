#pragma once
#include "Observable.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/trigonometric.hpp>

const float CAMERA_SPEED = 0.15f;

class Camera : public Observable {
private:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    uint width = 800;
    uint height = 600;

    float yaw = -90.0f;
    float pitch = 0.0f;
    float last_x = width / 2.0f;
    float last_y = height / 2.0f;
    bool first_mouse = true;
    const float mouse_sensitivity = 0.15f;
    float fov = 45.0f;

    float delta_time = 0.0f;
    float last_frame = 0.0f;
    void calculate_speed();

public:
    Camera(uint width, uint height) : width(width), height(height) {}
    Camera() {}
    glm::mat4 get_view();
    glm::mat4 get_projection();
    void move_forward(float camera_speed = CAMERA_SPEED);
    void move_backward(float camera_speed = CAMERA_SPEED);
    void move_left(float camera_speed = CAMERA_SPEED);
    void move_right(float camera_speed = CAMERA_SPEED);
    void move_mouse(double x_pos, double y_pos);
    void modify_fov(float offset);
    void notify_observers() override;
};
