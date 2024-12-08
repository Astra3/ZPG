#include <GL/glew.h>
#include "GLFW/glfw3.h"

#include "Application.hpp"
#include <iostream>

void Application::create_shaders() {
    auto tex_shader = std::make_shared<ShaderProgram>(std::ifstream("../src/shaders/texture_vertex.glsl"),
                                                      std::ifstream("../src/shaders/texture_fragment.glsl"));
    CAMERA->attach(tex_shader);
    this->shaders.insert({"tex_shader", tex_shader});

    auto shader = std::make_shared<ShaderProgram>(std::ifstream("../src/shaders/general_vertex.glsl"),
                                                  std::ifstream("../src/shaders/general_fragment.glsl"));
    CAMERA->attach(shader);
    this->shaders.insert({"normal_shader", shader});
}

void Application::create_scenes() {
    auto tex_shader = this->shaders.at("tex_shader");
    auto shader = this->shaders.at("normal_shader");

    auto grass_texture = std::make_shared<Texture>("../src/sources/grass.png");
    auto wood_texture = std::make_shared<Texture>("../src/sources/test.png");
    
    // two plains
    this->scenes.emplace_back();
    auto plain = std::make_shared<models::Plain>();
    this->scenes[TEXTURE_TEST].add_model(DrawableObject(plain, tex_shader, {}, grass_texture, Material()));
    this->scenes[TEXTURE_TEST].add_model(DrawableObject(
        plain, tex_shader, {std::make_shared<transf::Translate>(glm::vec3(0, 1, 0))}, wood_texture, Material()));

    // forest day
    auto dir_light = std::make_unique<lights::Directional>(glm::vec3(0));
    dir_light->light_strength = LightStrength{.ambient = glm::vec3(.1f), .diffuse = glm::vec3(0.1f)};
    this->scenes.emplace_back(std::move(dir_light), CAMERA);

    auto flashlight = std::make_unique<lights::Flashlight>(CAMERA);
    this->scenes[FOREST].lights->push_back(std::move(flashlight));

    // FIXME move this one
    auto point = std::make_unique<lights::Point>(glm::vec3(50, 10, 8));
    this->scenes[FOREST].lights->push_back(std::move(point));

    this->scenes[FOREST].apply_generator(generators::trees_bushes, shader, 300);

    this->scenes[FOREST].add_model(DrawableObject(
        plain, tex_shader,
        {std::make_shared<transf::Scale>(glm::vec3(80.0f)), std::make_shared<transf::Translate>(glm::vec3(0, 0.0f, 0))},
        grass_texture, Material()));

    this->scenes[FOREST].add_model(DrawableObject(std::make_shared<models::ObjectFile>("../src/sources/house.obj"),
                                            tex_shader, {std::make_shared<transf::Scale>(glm::vec3(4))},
                                            std::make_shared<Texture>("../src/sources/house.png"), Material()));
    this->scenes[FOREST].add_model(DrawableObject(
        std::make_shared<models::ObjectFile>("../src/sources/login.obj"), shader,
        {std::make_shared<transf::Scale>(glm::vec3(80)), std::make_shared<transf::Translate>(glm::vec3(-2, 2, -5))},
        Material()));

    for (auto &light : *scenes[FOREST].lights) {
        light->attach(tex_shader);
        light->attach(shader);
    }
}

Application::Application() {
    if (!glfwInit()) {
        std::cerr << "Could not start GLFW3!\n";
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Testing OpenGL", NULL, NULL);
    if (this->window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(this->window);

    glewInit();
    glEnable(GL_DEPTH_TEST);

    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow *window, int width, int height) {
        CAMERA->set_width_height(width, height);
        glViewport(0, 0, width, height);
    });
    glfwSetCursorPosCallback(this->window, [](GLFWwindow *window, double x_pos, double y_pos) {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            CAMERA->move_mouse(x_pos, y_pos);
        else
            CAMERA->reset_first_mouse();
    });
    glfwSetMouseButtonCallback(this->window, [](GLFWwindow *window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    });

    glfwSetScrollCallback(this->window,
                          [](GLFWwindow *, double, double y_offset) { CAMERA->modify_fov(-y_offset * 2.0f); });
}

void Application::add_rotating_trees() {
    // FIXME might be wrong
    auto &shader = this->shaders.at("normal_shader");

    auto rotation = std::make_shared<transf::RotateTime>();
    auto scale = std::make_shared<transf::Scale>(glm::vec3(2.0f));
    auto model_tree = std::make_shared<models::Tree>();
    scenes[FOREST].add_model(DrawableObject(
        model_tree, shader, {std::make_shared<transf::Translate>(glm::vec3(50.0f, 0.0f, 0.0f)), rotation, scale}));
    scenes[FOREST].add_model(DrawableObject(
        model_tree, shader, {std::make_shared<transf::Translate>(glm::vec3(0.0f, 0.0f, 50.0f)), rotation, scale}));
    scenes[FOREST].add_model(DrawableObject(
        model_tree, shader, {std::make_shared<transf::Translate>(glm::vec3(-50.0f, 0.0f, 0.0f)), rotation, scale}));
    scenes[FOREST].add_model(DrawableObject(
        model_tree, shader, {std::make_shared<transf::Translate>(glm::vec3(0.0f, 0.0f, -50.0f)), rotation, scale}));
}

void Application::run() {
    while (!glfwWindowShouldClose(this->window)) {
        float current_frame = glfwGetTime();
        this->delta_time = current_frame - this->last_frame;
        this->last_frame = current_frame;

        this->process_input();

        glClearColor(0.2f, .3f, .3f, .3f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->scenes[this->selected_scene].render();

        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }
    glfwTerminate();
}

void Application::process_input() {
    float camera_speed = 40.0f * this->delta_time;
    if (glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT)) {
        camera_speed *= 3;
    }
    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
        CAMERA->move_forward(camera_speed);
    }
    if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
        CAMERA->move_backward(camera_speed);
    }
    if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
        CAMERA->move_left(camera_speed);
    }
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
        CAMERA->move_right(camera_speed);
    }
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(this->window, true);
    }
    auto left = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
    auto right = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
    if ((left || right) && !held_scene) {
        this->held_scene = true;
        if (left) {
            if (this->selected_scene == 0)
                this->selected_scene = this->scenes.size() - 1;
            else
                this->selected_scene--;
        }

        if (right) {
            this->selected_scene++;
            if (this->selected_scene == this->scenes.size())
                this->selected_scene = 0;
        }
    } else if (!(left || right))
        this->held_scene = false;
}
