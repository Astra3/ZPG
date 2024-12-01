#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>

#include <cstddef>
#include <cstdlib>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "Camera.hpp"
#include "generators.hpp"
#include "models/tree.hpp"
#include "objects/DrawableObject.hpp"
#include "objects/Model.hpp"
#include "objects/ShaderProgram.hpp"
#include "observers/Observer.hpp"
#include "scene/Light.hpp"
#include "scene/Scene.hpp"

#define TEXTURE_TEST 0
#define FOREST 1

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

static auto CAMERA = std::make_shared<Camera>(SCR_WIDTH, SCR_HEIGHT);
void initialize_scenes(std::vector<Scene> &scenes, std::shared_ptr<ShaderProgram> shader) {
    auto tex_shader = std::make_shared<ShaderProgram>(std::ifstream("../src/shaders/texture_vertex.glsl"),
                                                      std::ifstream("../src/shaders/texture_fragment.glsl"));
    CAMERA->attach(tex_shader);
    auto grass_texture = std::make_shared<Texture>("../src/sources/grass.png");
    auto wood_texture = std::make_shared<Texture>("../src/sources/test.png");

    // two plains
    scenes.emplace_back();
    auto plain = std::make_shared<models::Plain>();
    scenes[TEXTURE_TEST].add_model(DrawableObject(plain, tex_shader, {}, grass_texture, Material()));
    scenes[TEXTURE_TEST].add_model(DrawableObject(
        plain, tex_shader, {std::make_shared<transf::Translate>(glm::vec3(0, 1, 0))}, wood_texture, Material()));

    // forest day
    auto dir_light = std::make_unique<lights::Directional>(glm::vec3(0));
    dir_light->light_strength = LightStrength{.ambient = glm::vec3(.1f), .diffuse = glm::vec3(0.1f)};
    scenes.emplace_back(std::move(dir_light), CAMERA);

    auto flashlight = std::make_unique<lights::Flashlight>(CAMERA);
    scenes[FOREST].lights->push_back(std::move(flashlight));

    auto point = std::make_unique<lights::Point>(glm::vec3(10, 10, 0));
    scenes[FOREST].lights->push_back(std::move(point));

    scenes[FOREST].apply_generator(generators::trees_bushes, shader, 300);

    scenes[FOREST].add_model(DrawableObject(
        plain, tex_shader,
        {std::make_shared<transf::Scale>(glm::vec3(80.0f)), std::make_shared<transf::Translate>(glm::vec3(0, 0.0f, 0))},
        grass_texture, Material()));

    scenes[FOREST].add_model(DrawableObject(std::make_shared<models::ObjectFile>("../src/sources/house.obj"),
                                            tex_shader, {std::make_shared<transf::Scale>(glm::vec3(4))},
                                            std::make_shared<Texture>("../src/sources/house.png"), Material()));
    scenes[FOREST].add_model(DrawableObject(
        std::make_shared<models::ObjectFile>("../src/sources/login.obj"), shader,
        {std::make_shared<transf::Scale>(glm::vec3(80)), std::make_shared<transf::Translate>(glm::vec3(-2, 2, -5))},
        Material()));

    for (auto &light : *scenes[FOREST].lights) {
        light->attach(tex_shader);
        light->attach(shader);
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Could not start GLFW3!\n";
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Testing OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glewInit();
    glEnable(GL_DEPTH_TEST);

    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        CAMERA->set_width_height(width, height);
        glViewport(0, 0, width, height);
    });

    std::vector<Scene> scenes;
    auto shader = std::make_shared<ShaderProgram>(std::ifstream("../src/shaders/general_vertex.glsl"),
                                                  std::ifstream("../src/shaders/general_fragment.glsl"));
    initialize_scenes(scenes, shader);
    size_t selected_scene = 0;
    CAMERA->attach(shader);

    auto delta_time = 0.0f;
    auto last_frame = 0.0f;

    auto process_input = [window, &delta_time, &selected_scene, &scenes, held_scene = false]() mutable {
        float camera_speed = 40.0f * delta_time;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
            camera_speed *= 3;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            CAMERA->move_forward(camera_speed);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            CAMERA->move_backward(camera_speed);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            CAMERA->move_left(camera_speed);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            CAMERA->move_right(camera_speed);
        }
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        auto left = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
        auto right = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
        if ((left || right) && !held_scene) {
            held_scene = true;
            if (left) {
                if (selected_scene == 0)
                    selected_scene = scenes.size() - 1;
                else
                    selected_scene--;
            }

            if (right) {
                selected_scene++;
                if (selected_scene == scenes.size())
                    selected_scene = 0;
            }
        } else if (!(left || right))
            held_scene = false;
    };

    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double x_pos, double y_pos) {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            CAMERA->move_mouse(x_pos, y_pos);
        else
            CAMERA->reset_first_mouse();
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    });
    glfwSetScrollCallback(window, [](GLFWwindow *, double, double y_offset) { CAMERA->modify_fov(-y_offset * 2.0f); });

    // auto rotation = std::make_shared<transf::RotateTime>();
    // auto scale = std::make_shared<transf::Scale>(glm::vec3(2.0f));
    // auto model_tree = std::make_shared<models::Tree>();
    // scenes[FOREST].add_model(DrawableObject(
    //     model_tree, shader, {std::make_shared<transf::Translate>(glm::vec3(8.0f, 0.0f, 0.0f)), rotation, scale}));
    // scenes[FOREST].add_model(DrawableObject(
    //     model_tree, shader, {std::make_shared<transf::Translate>(glm::vec3(0.0f, 0.0f, 8.0f)), rotation, scale}));
    // scenes[FOREST].add_model(DrawableObject(
    //     model_tree, shader, {std::make_shared<transf::Translate>(glm::vec3(-8.0f, 0.0f, 0.0f)), rotation, scale}));
    // scenes[FOREST].add_model(DrawableObject(
    //     model_tree, shader, {std::make_shared<transf::Translate>(glm::vec3(0.0f, 0.0f, -8.0f)), rotation, scale}));

    while (!glfwWindowShouldClose(window)) {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        process_input();

        glClearColor(0.2f, .3f, .3f, .3f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scenes[selected_scene].render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}
