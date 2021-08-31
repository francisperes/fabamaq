#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 3) in mat4 aInstanceMatrix;
layout (location = 7) in vec3 aColour;

out vec3 FragPos;
out vec3 Normal;
out vec3 Colour;

uniform mat4 mView;
uniform mat4 mProjection;

void main() {
    FragPos = vec3(aInstanceMatrix * vec4(aPos, 1.0));
    Colour = aColour;
    Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;
    gl_Position = mProjection * mView * vec4(FragPos, 1.0);
}
