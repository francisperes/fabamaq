#version 330
out vec4 FragColour;

struct Light {
    vec3 direction;
    vec3 colour;
};

in vec3 FragPos;
in vec3 Normal;
in vec3 Colour;

uniform vec3 viewPos;
uniform Light light;

void main() {
    vec3 ambient = 0.3 * light.colour;

    vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(-light.direction);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light.colour;


    vec3 view_dir = normalize(viewPos - FragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32); // 32 = material shininess
    vec3 specular = 0.8 * spec * light.colour;

    vec3 result = (ambient + diffuse + specular) * Colour;
    FragColour = vec4(result, 1.0);
}
