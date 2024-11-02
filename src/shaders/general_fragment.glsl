#version 330 core
in vec3 normal;
in vec3 frag_pos;
out vec4 frag_color;

uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main() {
    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_pos - frag_pos);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diff * light_color;

    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * light_color;

    float specular_strength = 0.6;
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 halfway_dir = normalize(light_direction + view_dir);

    float spec = pow(max(dot(view_dir, halfway_dir), 0.0), 128);
    vec3 specular = specular_strength * spec * light_color;

    vec3 result = (ambient + diffuse + specular) * normal;
    frag_color = vec4(result, 1.0);
}
