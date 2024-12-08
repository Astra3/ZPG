#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>

#include "generators.hpp"
#include "models/tree.hpp"
#include "Application.hpp"

int main() {
    Application app;

    app.create_shaders();
    app.create_scenes();

    app.add_rotating_trees();
    app.run();
    return 0;
}
