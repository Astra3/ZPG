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

    this->shader->use();
    this->model->render();
    glUseProgram(0);
}
