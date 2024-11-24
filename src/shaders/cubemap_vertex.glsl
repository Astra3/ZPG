#version 450

layout(location = 0) in vec3 a_tex_coords;

uniform mat4 view;
uniform mat4 projection;

out vec3 tex_coords;

void main() {
    gl_Position = projection * view * vec4(a_tex_coords, 1.0);
    tex_coords = a_tex_coords;
}

