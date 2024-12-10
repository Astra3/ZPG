#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>

#include "Application.hpp"
#include "generators.hpp"
#include "models/tree.hpp"

int main() {
    Application app;

    app.create_scenes();

    app.run();
    return 0;
}
