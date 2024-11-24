#version 450

in vec3 tex_coords;
out vec4 frag_color;

uniform samplerCube cubemap;

void main() {
    frag_color = texture(cubemap, tex_coords);
}

