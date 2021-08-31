#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include <iostream>
#include <glm/glm.hpp>

namespace physics {
    struct SphereCollider {
        glm::vec3 p;
        float r;
    };

    namespace math {
        // Intersects ray r = p + td, |d| = 1, with sphere s and, if intersecting,
        // return t value of interseciton
        inline int IntersectRaySphere(glm::vec3 p, glm::vec3 d, SphereCollider s, float &t) {
            glm::vec3 m = p - s.p;
            float b = glm::dot(m, d);
            float c = glm::dot(m, m) - s.r * s.r;
            // Exit if r's origin is outside s (c > 0) and r pointing away from s (b > 0)
            if (c > 0.0f && b > 0.0f) return 0;
            float discr = b * b - c;
            // A negative discriminant corresponds to ray missing sphere
            if (discr < 0.0f) return 0;
            // Ray now found to intersect sphere, comput smallets t value of intersection
            t = -b - sqrt(discr);
            // If t is negative, ray started inside sphere so clamp t to 0
            if (t < 0.0f) { t = 0.0f; }
            return 1;
        }
    }
}

#endif
