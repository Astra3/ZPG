#include "Camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera::Camera() {
    glm::mat4 view(1.0f);
    this->view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
    this->projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}

glm::mat4 Camera::get_projection() {
    return this->projection;
}

glm::mat4 Camera::get_view() {
    return this->view;
}

void Camera::notify_observers() {
    for (auto &observer : this->observers) {
        observer->update(*this);
    }
}

