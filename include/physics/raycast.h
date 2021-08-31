#ifndef RAYCAST_H
#define RAYCAST_H

#include <glm/glm.hpp>

namespace physics {
    struct Raycast {
        glm::vec3 p;
        glm::vec3 d;
    };

    namespace math {
        inline Raycast RaycastFromCamera(int mouse_x, int mouse_y,
                glm::vec3 camera_pos, glm::mat4 projection_matrix,
                float window_width, float window_height) {
            float x = (2.0f * mouse_x) / window_width - 1.0f;
            float y = 1.0f - (2.0f * mouse_y) / window_height;
            float z = -1.0f;
            glm::vec4 ray_clip = glm::normalize(glm::vec4(x, y, z, 1.0f));
            glm::vec4 ray_eye = glm::inverse(projection_matrix) * ray_clip;
            glm::vec3 d = glm::normalize(glm::vec3(ray_eye));

            Raycast res;
            res.p = camera_pos;
            res.d = d;
            return res;
        }
    }
}

#endif
