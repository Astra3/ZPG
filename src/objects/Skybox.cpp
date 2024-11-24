#include "Skybox.hpp"
#include "../Camera.hpp"
#include "Model.hpp"
#include "ShaderProgram.hpp"

void SkyBox::render() {
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    this->shader_program.apply_transformation("view", glm::mat4(glm::mat3(this->camera->get_view())));
    this->shader_program.apply_transformation("projection", this->camera->get_projection());

    this->cubemap.apply_uniform(this->shader_program);
    this->cubemap.bind();

    this->shader_program.use();
    this->skycube.render();
    glUseProgram(0);
}

