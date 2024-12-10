#version 330 core
#define MAX_LIGHTS 1
in vec3 normal;
in vec3 frag_pos;
in vec2 tex_coord;
out vec4 frag_color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 specular;
    vec3 diffuse;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 specular;
    vec3 diffuse;
};

struct SpotLight {
    vec3 direction;
    vec3 position;
    float cut_off;
    float outer_cut_off;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 specular;
    vec3 diffuse;
};

uniform PointLight point_lights[MAX_LIGHTS];
uniform DirectionalLight directional_lights[MAX_LIGHTS];
uniform SpotLight spot_lights[MAX_LIGHTS];

uniform sampler2D tex_unit_id;

uniform uint point_light_count;
uniform uint dir_light_count;
uniform uint spot_light_count;

uniform Material material;

uniform vec3 view_pos;

vec3 calc_direction_light(DirectionalLight light, vec3 view_dir, vec3 norm) {
    vec3 light_direction = normalize(-light.direction);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 ambient = light.ambient * material.ambient;

    vec3 halfway_dir = normalize(light_direction + view_dir);

    float spec = pow(max(dot(norm, halfway_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    return (ambient + diffuse + specular);
}

vec3 calc_point_light(PointLight light, vec3 view_dir, vec3 norm) {
    vec3 light_direction = normalize(light.position - frag_pos);
    vec3 ambient = light.ambient * material.ambient;

    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 halfway_dir = normalize(light_direction + view_dir);

    float spec = pow(max(dot(norm, halfway_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calc_spot_light(SpotLight light, vec3 view_dir, vec3 norm) {
    vec3 light_direction = normalize(light.position - frag_pos);
    vec3 ambient = light.ambient * material.ambient;

    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 halfway_dir = normalize(light_direction + view_dir);

    float spec = pow(max(dot(norm, halfway_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(light_direction, normalize(-light.direction));
    float epsilon = light.cut_off - light.outer_cut_off;
    float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;

    // ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main() {
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 norm = normalize(normal);
    // vec3 result = calc_point_light(point_lights[0], view_dir) * normal;
    vec3 result = vec3(0);
    for (uint i = 0u; i < point_light_count; i++) {
        result += calc_point_light(point_lights[i], view_dir, norm);
    }

    for (uint i = 0u; i < spot_light_count; i++) {
        result += calc_spot_light(spot_lights[i], view_dir, norm);
    }

    for (uint i = 0u; i < dir_light_count; i++) {
        result += calc_direction_light(directional_lights[i], view_dir, norm);
    }

    // frag_color = vec4(result, 1.0), texture(our_texture, tex_coord);
    frag_color = vec4(result, 1.0) * texture(tex_unit_id, tex_coord);
}
