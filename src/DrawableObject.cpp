#include "DrawableObject.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void DrawableObject::render() {
    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    int id_loc;
    if (this->transformations.has_value()) {
        for (auto &transformation : *this->transformations) {
            transformation->apply(model);
        }
    }
    this->shader->apply_transformation("model", model);
    if (this->camera.has_value()) {
        this->shader->update(**this->camera);
    }

    this->shader->use();
    this->model->render();
}
