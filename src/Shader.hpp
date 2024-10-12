#include <GL/glew.h>

#pragma once

class Shader {
private:
    GLuint shader_program_id;
public:
    Shader(const char* vertex_shader, const char* fragment_shader);
    void use();
    void unuse();
    ~Shader();
};

