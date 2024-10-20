#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>

#include <cstddef>
#include <cstdlib>
#include <glm/trigonometric.hpp>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "DrawableObject.hpp"
#include "Model.hpp"
#include "models/tree.hpp"

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
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
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Testing OpenGL", NULL, NULL);
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

    // glEnable(GL_DEPTH_TEST);

    const float texCoords[] = {
        0.0f, 0.0f, // lower-left corner
        1.0f, 0.0f, // lower-right corner
        0.5f, 1.0f  // top-center corner
    };
    TransformationType t_first;
    t_first.push_back(std::make_unique<transf::Translate>(transf::Translate(glm::vec3(-2.0f, -4.0f, -0.0f))));

    TransformationType t_second;
    t_second.push_back(std::make_unique<transf::Translate>(transf::Translate(glm::vec3(2.0f, -4.0f, -3.0f))));
    
    auto shader =
        std::make_shared<Shader>(std::ifstream("../src/shaders/tree.vs"), std::ifstream("../src/shaders/tree.fs"));
    auto model = std::make_shared<models::Tree>();

    DrawableObject object1(model, shader, std::move(t_first));
    DrawableObject object2(model, shader, std::move(t_second));
    // DrawableObject object(std::make_unique<models::Sphere>(),
    //                       std::make_unique<Shader>(std::ifstream("../src/shaders/sphere.vert"),
    //                                                std::ifstream("../src/shaders/sphere.frag")));

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, .3f, .3f, .3f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glm::mat4 trans(1.0f);

        // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        // trans = glm::scale(trans, glm::vec3(0.1, 0.1, 0.1));
        // shader_program.apply_transformation("transform", trans);

        // TODO leave this for later lol
        // glm::mat4 projection_matrix(1.0f);
        // glm::mat4 view_matrix(1.0f);
        // glm::mat4 model_matrix(1.0f);
        // shader_program.apply_transformation("projection_matrix", projection_matrix);
        // shader_program.apply_transformation("view_matrix", view_matrix);
        // shader_program.apply_transformation("model_matrix", model_matrix);
        // glDrawArrays(GL_TRIANGLES, 0, (tree.size() - 1));
        object1.render();
        object2.render();

        // violet_program.use();
        // vao2.bind();
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}
