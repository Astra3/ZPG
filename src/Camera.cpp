#include "Camera.hpp"
#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

glm::mat4 Camera::get_projection() {
    return glm::perspective(glm::radians(fov), (float)this->width / (float)this->height, 0.1f, 10000.0f);
}

glm::mat4 Camera::get_view() { return glm::lookAt(this->position, this->position + this->front, this->up); }

void Camera::move_forward(float camera_speed) { this->position += camera_speed * this->front; }

void Camera::move_backward(float camera_speed) { this->position -= camera_speed * this->front; }

void Camera::move_left(float camera_speed) {
    this->position -= glm::normalize(glm::cross(this->front, this->up)) * camera_speed;
}

void Camera::move_right(float camera_speed) {
    this->position += glm::normalize(glm::cross(this->front, this->up)) * camera_speed;
}

void Camera::move_mouse(double x_pos, double y_pos) {
    if (this->first_mouse) {
        this->last_x = x_pos;
        this->last_y = y_pos;
        this->first_mouse = false;
    }
    float x_offset = x_pos - this->last_x;
    float y_offset = this->last_y - y_pos;
    this->last_x = x_pos;
    this->last_y = y_pos;

    x_offset *= this->mouse_sensitivity;
    y_offset *= this->mouse_sensitivity;

    this->yaw += x_offset;
    this->pitch += y_offset;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
    direction.y = std::sin(glm::radians(pitch));
    direction.z = std::sin(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
    this->front = glm::normalize(direction);
}

void Camera::modify_fov(float offset) {
    this->fov += offset;
    if (fov < 1.0f) {
        this->fov = 1.0f;
    }
    if (fov > 120.0f) {
        this->fov = 120.0f;
    }
}

void Camera::notify_observers() {
    for (auto &observer : this->observers) {
        observer->update(*this);
    }
}
