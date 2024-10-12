#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include "Shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Testing OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW windw" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    glewExperimental = GL_TRUE;
    glewInit();

    auto points = {
        0.0f,  0.8f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.3f, -0.5f, 0.0f
    };
    auto points2 = {
       -0.8f,  0.8f, 0.0f,  // top
       -0.8f,  0.2f, 0.0f,  // right
       -1.0f, -0.0f, 0.0f   // left
    };
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(0.5, 1.0, 0.5, 1.0);\n"
        "}\0";

    const char *fragmentShaderSourceViolet = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0, 0.0, 1.0, 1.0);\n"
        "}\0";

    Shader shader_program(vertexShaderSource, fragmentShaderSource);
    Shader violet_program(vertexShaderSource, fragmentShaderSourceViolet);

    VAO vao1;
    vao1.bind();
    VBO vbo1(points);
    vao1.link_vbo(vbo1, 0);

    VAO vao2;
    vao2.bind();
    VBO vbo2(points2);
    vao2.link_vbo(vbo2, 0);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, .3f, .3f, .3f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.use();
        vao1.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        violet_program.use();
        vao2.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}

