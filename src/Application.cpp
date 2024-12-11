#include "objects/Model.hpp"
#include "objects/ShaderProgram.hpp"
#include "scene/Light.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Application.hpp"
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include <memory>

void Application::create_scenes() {
    this->scenes.reserve(3);

    auto grass_texture = std::make_shared<Texture>("../src/sources/grass.png");
    auto wood_texture = std::make_shared<Texture>("../src/sources/test.png");

    // two plains
    auto test_light = std::make_unique<lights::Directional>(glm::vec3(0));
    test_light->light_strength.ambient = glm::vec3(1.f);
    this->scenes.emplace_back(std::move(test_light));
    auto test_shader = ShaderProgram::create_texture(*CAMERA);
    auto plain = std::make_shared<models::Plain>();
    this->scenes[TEXTURE_TEST].add_model(DrawableObject(plain, test_shader, {}, grass_texture, Material()));
    this->scenes[TEXTURE_TEST].add_model(DrawableObject(
        plain, test_shader, {std::make_shared<transf::Translate>(glm::vec3(0, 1, 0))}, wood_texture, Material()));

    for (auto &light : *scenes[TEXTURE_TEST].lights) {
        light->attach(test_shader);
    }

    // forest day
    auto forest_shader = ShaderProgram::create_normal_phong(*CAMERA);
    auto forest_tex_shader = ShaderProgram::create_texture(*CAMERA);
    auto dir_light = std::make_unique<lights::Directional>(glm::vec3(0));
    dir_light->light_strength = LightStrength{.ambient = glm::vec3(.1f), .diffuse = glm::vec3(0.1f)};
    this->scenes.emplace_back(std::move(dir_light), CAMERA);

    auto flashlight = std::make_unique<lights::Flashlight>(CAMERA);
    flashlight->attenuation.quadratic = 0.008f;
    this->scenes[FOREST].lights->push_back(std::move(flashlight));

    auto point = std::make_unique<lights::Point>(glm::vec3(50, 10, 8));
    this->scenes[FOREST].lights->push_back(std::move(point));

    this->scenes[FOREST].apply_generator(generators::trees_bushes, forest_shader, 300);

    this->scenes[FOREST].add_model(DrawableObject(
        plain, forest_tex_shader,
        {std::make_shared<transf::Scale>(glm::vec3(80.0f)), std::make_shared<transf::Translate>(glm::vec3(0, 0.0f, 0))},
        grass_texture, Material()));

    this->scenes[FOREST].add_model(DrawableObject(std::make_shared<models::ObjectFile>("../src/sources/house.obj"),
                                                  forest_tex_shader, {std::make_shared<transf::Scale>(glm::vec3(4))},
                                                  std::make_shared<Texture>("../src/sources/house.png"), Material()));
    this->scenes[FOREST].add_model(DrawableObject(
        std::make_shared<models::ObjectFile>("../src/sources/login.obj"), forest_tex_shader,
        {std::make_shared<transf::Scale>(glm::vec3(80)), std::make_shared<transf::Translate>(glm::vec3(-2, 2, -5))},
        wood_texture, Material()));

    auto rotation = std::make_shared<transf::RotateTime>();
    auto scale = std::make_shared<transf::Scale>(glm::vec3(2.0f));
    auto model_tree = std::make_shared<models::Tree>();
    scenes[FOREST].add_model(
        DrawableObject(model_tree, forest_shader,
                       {std::make_shared<transf::Translate>(glm::vec3(50.0f, 0.0f, 0.0f)), rotation, scale}));
    scenes[FOREST].add_model(
        DrawableObject(model_tree, forest_shader,
                       {std::make_shared<transf::Translate>(glm::vec3(0.0f, 0.0f, 50.0f)), rotation, scale}));
    scenes[FOREST].add_model(
        DrawableObject(model_tree, forest_shader,
                       {std::make_shared<transf::Translate>(glm::vec3(-50.0f, 0.0f, 0.0f)), rotation, scale}));
    scenes[FOREST].add_model(
        DrawableObject(model_tree, forest_shader,
                       {std::make_shared<transf::Translate>(glm::vec3(0.0f, 0.0f, -50.0f)), rotation, scale}));
    for (auto &light : *scenes[FOREST].lights) {
        light->attach(forest_tex_shader);
        light->attach(forest_shader);
    }

    // four spheres
    auto second_shader = ShaderProgram::create_normal_blinn(*CAMERA);
    second_shader->apply_transformation("is_white", true);
    auto phong = ShaderProgram::create_normal_phong(*CAMERA);
    phong->apply_transformation("is_white", true);

    auto sphere_point = std::make_unique<lights::Point>(glm::vec3(0));
    sphere_point->attach(second_shader);
    sphere_point->attach(phong);
    this->scenes.emplace_back(std::move(sphere_point));

    auto sphere = std::make_shared<models::Sphere>();
    auto positions1 = {glm::vec3(2, 0, 2), glm::vec3(-2, 0, -2)};
    auto positions2 = {glm::vec3(2, 0, -2), glm::vec3(-2, 0, 2)};
    auto positions3 = {glm::vec3(0, 3, 0), glm::vec3(0, -3, 0)};
    auto scale_spheres = std::make_shared<transf::Scale>(glm::vec3(4));
    Material shiny_material;
    shiny_material.specular = glm::vec3(8.f);
    shiny_material.shininess = 128.f;
    Material diffuse_material;
    diffuse_material.diffuse = glm::vec3(1.5f);
    for (auto pos : positions1) {
        this->scenes[FOUR_SPHERES].add_model(DrawableObject(
            sphere, second_shader, {scale_spheres, std::make_shared<transf::Translate>(pos)}, shiny_material));
    }
    for (auto pos : positions2) {
        this->scenes[FOUR_SPHERES].add_model(DrawableObject(
            sphere, second_shader, {scale_spheres, std::make_shared<transf::Translate>(pos)}, diffuse_material));
    }
    for (auto pos : positions3) {
        this->scenes[FOUR_SPHERES].add_model(
            DrawableObject(sphere, phong, {scale_spheres, std::make_shared<transf::Translate>(pos)}, shiny_material));
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

    this->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ZPG Projekt 😎😭", NULL, NULL);
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
    auto left = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
    auto right = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
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
