#ifndef WORLD_H
#define WORLD_H

#include <glm/glm.hpp>
#include <vector>
#include "sphere_collider.h"
#include "cylindrical_helix.h"
#include "raycast.h"

#define MAX_ENTITIES 160

namespace physics {
    class World {
    public:
        World();
        ~World();

        void Update(float delta);
        std::vector<glm::vec3> GetPositions();
        int CheckCollision(glm::vec3 p, glm::vec3 d);
    private:
        std::vector<glm::vec3> m_positions;
        std::vector<glm::vec3> m_inicialPositions;
        std::vector<float> m_timers;
        std::vector<SphereCollider> m_spheres;
    };
}

#endif
