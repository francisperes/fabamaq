#include "renderer/light.h"

namespace renderer {
    Light::Light(glm::vec3 direction, glm::vec3 colour) {
        m_direction = direction;
        m_colour = colour;
    }   

    void Light::Render(Shader* p_shader) {
        p_shader->Use();
        // Since we only have one object lets assume all materials have the
        // same shininess, ambient, diffuse and specular proprieties
        p_shader->SetVec3("light.direction", m_direction);
        p_shader->SetVec3("light.colour", m_colour);
    }
}
