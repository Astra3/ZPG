#include "Transformation.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

namespace transf {
void Rotate::apply(glm::mat4 &matrix) { matrix = glm::rotate(matrix, this->angle, this->axis); }

void Rotate::set_angle(float angle) {
    this->angle = angle;
}

void Translate::apply(glm::mat4 &matrix) { matrix = glm::translate(matrix, this->translation_vec); }

void Scale::apply(glm::mat4 &matrix) { matrix = glm::scale(matrix, this->scale_ratio); }
} // namespace transf
