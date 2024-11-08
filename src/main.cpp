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
#include "observers/Observer.hpp"
#include "scene/Light.hpp"
#include "scene/Scene.hpp"

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

static auto CAMERA = std::make_shared<Camera>(SCR_WIDTH, SCR_HEIGHT);
void initialize_scenes(std::vector<Scene> &scenes, std::shared_ptr<ShaderProgram> shader) {
    // auto light = std::make_shared<lights::PositionedLight>(glm::vec3(1.0f), glm::vec3(0.0f));
    auto light = std::make_unique<lights::PositionedLight>(glm::vec3(1.0f), glm::vec3(0.0f));
    light->attach(shader);
    scenes.push_back(Scene(std::move(light)));

    auto second_light = std::make_unique<lights::PositionedLight>(glm::vec3(1.0f, 1.0f, 0), glm::vec3(0));
    second_light->attach(shader);
    scenes[0].add_light(std::move(second_light));

    scenes[0].apply_generator(generators::trees_bushes, shader, 300);

    light = std::make_unique<lights::PositionedLight>(glm::vec3(1.0f), glm::vec3(0.0f));
    light->attach(shader);
    scenes.push_back(Scene(std::move(light)));

    auto sphere = std::make_shared<models::Sphere>();
    auto positions = {glm::vec3(8, 0, 8), glm::vec3(8, 0, -8), glm::vec3(-8, 0, 8), glm::vec3(-8, 0, -8)};
    for (auto pos : positions) {
        scenes[1].add_model(DrawableObject(
            sphere, shader, {std::make_unique<transf::Translate>(pos), std::make_unique<transf::Scale>(glm::vec3(4))}));
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Could not start GLFW3!\n";
        return EXIT_FAILURE;
    }
    glEnable(GL_DEPTH_TEST);
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

    glfwSetFramebufferSizeCallback(window,
                                   [](GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); });

    glewExperimental = GL_TRUE;
    glewInit();

    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);

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

    auto rotation = std::make_shared<transf::Rotate>(0, glm::vec3(0.0f, 1.0f, 0.0f));
    auto scale = std::make_shared<transf::Scale>(glm::vec3(2.0f));
    auto model_tree = std::make_shared<models::Tree>();
    scenes[0].add_model(DrawableObject(
        model_tree, shader, {std::make_shared<transf::Translate>(glm::vec3(8.0f, -10.0f, 0.0f)), rotation, scale}));
    scenes[0].add_model(DrawableObject(
        model_tree, shader, {std::make_shared<transf::Translate>(glm::vec3(0.0f, -10.0f, 8.0f)), rotation, scale}));
    scenes[0].add_model(DrawableObject(
        model_tree, shader, {std::make_shared<transf::Translate>(glm::vec3(-8.0f, -10.0f, 0.0f)), rotation, scale}));
    scenes[0].add_model(DrawableObject(
        model_tree, shader, {std::make_shared<transf::Translate>(glm::vec3(0.0f, -10.0f, -8.0f)), rotation, scale}));

    auto num = -8.f;
    bool go_back = false;
    while (!glfwWindowShouldClose(window)) {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        process_input();

        glClearColor(0.2f, .3f, .3f, .3f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (go_back)
            num -= .1;
        else
            num += .1;

        if (num > 8)
            go_back = true;
        else if (num < -8)
            go_back = false;
        auto pos = glm::vec3(num, 0.0f, num);
        dynamic_cast<lights::PositionedLight *>(&*scenes[0].lights.value()[0])->set_position(pos);
        dynamic_cast<lights::PositionedLight *>(&*scenes[0].lights.value()[1])
            ->set_position(glm::vec3(-num + 2, 0.0f, -num));
        rotation->set_angle(glfwGetTime() * 8);
        scenes[selected_scene].render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}
