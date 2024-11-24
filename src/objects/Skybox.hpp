#include "DrawableObject.hpp"
#include "Model.hpp"
#include "ShaderProgram.hpp"
class SkyBox {
private:
    ShaderProgram shader_program;
    std::shared_ptr<Camera> camera;
    CubeMap cubemap;
    models::SkyCube skycube;

    glm::mat4 previous_view;
    glm::mat4 previous_projection;

public:
    bool lock = false;
    SkyBox(std::shared_ptr<Camera> camera)
        : camera(camera), shader_program(std::ifstream("../src/shaders/cubemap_vertex.glsl"),
                                         std::ifstream("../src/shaders/cubemap_fragment.glsl")),
          cubemap("../src/img/posx.jpg", "../src/img/negx.jpg", "../src/img/posy.jpg", "../src/img/negy.jpg",
                  "../src/img/posz.jpg", "../src/img/negz.jpg") {}
    void render();
};
