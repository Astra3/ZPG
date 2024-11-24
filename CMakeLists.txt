cmake_minimum_required(VERSION 3.10)

project(MyCppProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OpenGL_GL_PREFERENCE LEGACY)

find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

# find_package(glfw3 REQUIRED)
add_subdirectory(libs/glfw)
add_subdirectory(libs/soil2)

find_package(GLEW REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

include_directories(include)

add_executable(zpg ${SOURCES})

target_link_libraries(zpg PRIVATE soil2 glfw glm::glm ${OPENGL_gl_LIBRARY} GLEW::GLEW)

