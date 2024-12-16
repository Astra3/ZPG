#version 330

in vec2 tex_coord;
out vec4 frag_color;
uniform sampler2D tex_unit_id;

void main() {
    frag_color = texture(tex_unit_id, tex_coord);
}

