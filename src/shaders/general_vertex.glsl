#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 frag_pos;
out vec3 normal;

void main() {
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    frag_pos = vec3(model * vec4(a_position, 1.0));
    // i read that in a good program, inversing should be done on the CPU to not bother the GPU too much
    // but hardware didn't advance for nothing
    // TODO is this supposed to be normalized?
    normal = mat3(transpose(inverse(model))) * a_normal;
    // normal = a_normal;
}

