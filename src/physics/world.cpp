#include "physics/world.h"
#include <stdio.h>

namespace physics {
    World::World() {
        // Generate entities
        int columns = 10;
        int entities_per_column = MAX_ENTITIES / columns;
        for (int i = 0; i < columns; i++) {
            for (int j = 0; j < entities_per_column; j++) {
                float t = j;
                glm::vec3 pos = glm::vec3((float)(i - 5), -8.0f, 0.0f);
                pos += math::CylindricalHelixPoint(0.5f, 0.5f, t);
                SphereCollider s;
                s.p = pos;
                s.r = 0.2f;

                m_positions.push_back(pos);
                m_inicialPositions.push_back(pos);
                m_timers.push_back(t);
                m_spheres.push_back(s);
            }
        }

        // Force one update
        Update(8.0f / 1000.0f);
    }
    World::~World() {}

    void World::Update(float delta) {
        for (int i = 0; i < MAX_ENTITIES; i++) {
            m_positions[i] = m_inicialPositions[i] + math::CylindricalHelixPoint(0.5f, 0.1f, 4 * m_timers[i]);
            if (m_positions[i].y > 8.0f) { m_inicialPositions[i].y -= 15.0f; }
            m_spheres[i].p = m_positions[i];
            m_timers[i] += delta;

        }
    }

    int World::CheckCollision(glm::vec3 p, glm::vec3 d) {
        float t;
        float smallest_t = std::numeric_limits<float>::max();
        int smallest_t_index = -1;
        for (int i = 0; i < MAX_ENTITIES; i++) {
            if (math::IntersectRaySphere(p, d, m_spheres[i], t)) {
                if (t < smallest_t) {
                    std::cout << i << std::endl;
                    smallest_t_index = i;
                    smallest_t = t;
                }
            }
        }
        return smallest_t_index;
    }

    std::vector<glm::vec3> World::GetPositions() { return m_positions; }
}
