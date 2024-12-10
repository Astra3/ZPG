#include "DrawableObject.hpp"
#include "Model.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void DrawableObject::render() {
    glm::mat4 model(1.0f);
    if (this->transformations.has_value()) {
        for (auto &transformation : *this->transformations) {
            transformation->apply(model);
        }
    }
    this->shader->apply_transformation("model", model);

    if (this->texture.has_value()) {
        // this->shader->apply_transformation("tex_unit_id", 0);
        (*this->texture)->apply_uniform(*this->shader);
        (*this->texture)->bind();
    }

    if (this->material.has_value()) {
        // FIXME not reset if there is no value
        this->material->apply(*this->shader);
    }

    this->shader->render();
    this->model->render();
    glUseProgram(0);
}
