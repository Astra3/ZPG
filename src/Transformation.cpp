#include "Transformation.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

namespace transf {
    void Rotate::apply(glm::mat4 &matrix) {
        matrix = glm::rotate(matrix, this->angle, this->axis);
    }

    void Translate::apply(glm::mat4 &matrix) {
        matrix = glm::translate(matrix, this->translation_vec);
    }
}

