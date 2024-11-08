#version 330 core
#define MAX_LIGHTS 4
in vec3 normal;
in vec3 frag_pos;
out vec4 frag_color;

struct PointLight {
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

uniform PointLight point_lights[MAX_LIGHTS];

uniform vec3 view_pos;

vec3 calc_point_light(PointLight light, vec3 view_dir, vec3 norm) {
    vec3 light_direction = normalize(light.position - frag_pos);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diff * light.color;

    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * light.color;

    float specular_strength = 0.6;
    vec3 halfway_dir = normalize(light_direction + view_dir);

    float spec = pow(max(dot(view_dir, halfway_dir), 0.0), 128);
    vec3 specular = specular_strength * spec * light.color;

    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main() {
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 norm = normalize(normal);
    // vec3 result = calc_point_light(point_lights[0], view_dir) * normal;
    vec3 result = vec3(0);
    for (int i = 0; i < 2; i++) {
        result += calc_point_light(point_lights[i], view_dir, norm);
    }
    result *= normal;
    frag_color = vec4(result, 1.0);
}
