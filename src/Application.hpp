#include "Camera.hpp"
#include "GLFW/glfw3.h"
#include "scene/Scene.hpp"
#include <memory>

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
#define TEXTURE_TEST 0
#define FOREST 1

static auto CAMERA = std::make_shared<Camera>(SCR_WIDTH, SCR_HEIGHT);

class Application {
private:
    std::vector<Scene> scenes;
    std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaders;
    size_t selected_scene = 0;
    float delta_time = 0.0f;
    float last_frame = 0.0f;

    bool held_scene = false;
    GLFWwindow *window;
    void process_input();
public:
    Application();
    void create_shaders();
    void create_scenes();
    void add_rotating_trees();
    void run();
};
