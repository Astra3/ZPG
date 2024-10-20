#version 330 core
in vec3 color_normal;
out vec4 frag_color;

void main() {
    vec3 color = normalize(color_normal) * 0.5 + 0.5;
    frag_color = vec4(color, 1.0);
}
