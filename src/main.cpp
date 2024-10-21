#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "Camera.hpp"
#include "DrawableObject.hpp"
#include "Model.hpp"
#include "models/tree.hpp"

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

static auto CAMERA = std::make_shared<Camera>(SCR_WIDTH, SCR_HEIGHT);

int main() {
    if (!glfwInit()) {
        std::cerr << "Could not start GLFW3!\n";
        return EXIT_FAILURE;
    }
    glEnable(GL_DEPTH_TEST);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Testing OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW windw" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window,
                                   [](GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); });

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);

    auto delta_time = 0.0f;
    auto last_frame = 0.0f;

    auto process_input = [window, &delta_time]() {
        float camera_speed = 40.0f * delta_time;
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
    };

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window,
                             [](GLFWwindow *, double x_pos, double y_pos) { CAMERA->move_mouse(x_pos, y_pos); });
    glfwSetScrollCallback(window, [](GLFWwindow *, double, double y_offset) { CAMERA->modify_fov(-y_offset * 2.0f); });

    const float texCoords[] = {
        0.0f, 0.0f, // lower-left corner
        1.0f, 0.0f, // lower-right corner
        0.5f, 1.0f  // top-center corner
    };

    auto shader =
        std::make_shared<Shader>(std::ifstream("../src/shaders/tree.vs"), std::ifstream("../src/shaders/tree.fs"));
    auto model_tree = std::make_shared<models::Tree>();
    auto model_bush = std::make_shared<models::Bush>();

    // DrawableObject object1(model_tree, shader, CAMERA, std::move(t_first));
    // DrawableObject object2(model_tree, shader, CAMERA, std::move(t_second));
    std::random_device rand_dev;
    std::default_random_engine e1(rand_dev());
    std::uniform_real_distribution<float> degrees(0, 360);
    std::uniform_real_distribution<float> position(-80, 80);
    std::uniform_real_distribution<float> scale(0.5, 4);

    std::vector<DrawableObject> objects;
    objects.reserve(300);
    for (size_t i = 0; i < 100; i++) {
        TransformationType t[3];
        for (size_t j = 0; j < 3; j++) {
            t[j].push_back(
                std::make_unique<transf::Translate>(transf::Translate(glm::vec3(position(e1), -10.0f, position(e1)))));
            t[j].push_back(std::make_unique<transf::Scale>(transf::Scale(glm::vec3(scale(e1)))));
            t[j].push_back(std::make_unique<transf::Rotate>(transf::Rotate(degrees(e1), glm::vec3(0.0f, 1.0f, 0.0f))));
        }
        objects.push_back(DrawableObject(model_tree, shader, CAMERA, std::move(t[0])));
        objects.push_back(DrawableObject(model_bush, shader, CAMERA, std::move(t[1])));
        objects.push_back(DrawableObject(model_bush, shader, CAMERA, std::move(t[2])));
    }

    while (!glfwWindowShouldClose(window)) {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        process_input();

        glClearColor(0.2f, .3f, .3f, .3f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::ranges::for_each(objects, &DrawableObject::render);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}
