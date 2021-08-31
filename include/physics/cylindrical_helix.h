#ifndef CYLINDRICAL_HELIX_H
#define CYLINDRICAL_HELIX_H

#include <glm/glm.hpp>
#include <cmath>

namespace physics {
    namespace math {
        // r = cylinder radious, c = spiral y velocity, t = time
        inline glm::vec3 CylindricalHelixPoint(float r, float c, float t) {
            float x = r * cosf(t);
            float y = c * t;
            float z = r * sinf(t);
            glm::vec3 res = glm::vec3(x, y, z);

            return res;
        }
    }
}

#endif
