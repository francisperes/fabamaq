#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "renderer/shader.h"

namespace renderer {
    class Light {
    public:
        Light(glm::vec3 direction, glm::vec3 colour);
        void Render(Shader* p_shader);
    private:
        glm::vec3 m_direction;
        glm::vec3 m_colour;
    };
}

#endif
