#ifndef SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>

#include "shader.h"

namespace renderer {

    class Sphere {
    public:
        Sphere(Shader* p_shader, float radius = 0.5f, int sector_count = 32, int stack_count = 18) {
            float x, y, z, xy;
            float nx, ny, nz, length_inv = 1.0f / radius;
            
            float sector_step = 2 * M_PI / sector_count;
            float stack_step = M_PI / stack_count;
            float sector_angle, stack_angle;

            for (int i = 0; i <= stack_count; i++) {
                stack_angle = M_PI / 2 - i * stack_step;
                xy = radius * cosf(stack_angle);
                z = radius * sinf(stack_angle);

                // Add (sector_count + 1) vertices per stack
                for (int j = 0; j <= sector_count; j++) {
                    sector_angle = j * sector_step;

                    // Vertex positions
                    x = xy * cosf(sector_angle);
                    y = xy * sinf(sector_angle);
                    m_vertices.push_back(x);
                    m_vertices.push_back(y);
                    m_vertices.push_back(z);

                    // Normalized vertex data
                    nx = x * length_inv;
                    ny = y * length_inv;
                    nz = z * length_inv;
                    m_normals.push_back(nx);
                    m_normals.push_back(ny);
                    m_normals.push_back(nz);
                }
            }

            // Indices
            //
            // k1--k1+1
            // | \   |
            // |  \  |
            // |   \ |
            // k2--k2+1
            for (int i = 0; i < stack_count; i++) {
                float k1 = i * (sector_count + 1);
                float k2 = k1 + sector_count + 1;

                for (int j = 0; j < sector_count; j++, k1++, k2++) {
                    if (i != 0) {
                        m_indices.push_back(k1);
                        m_indices.push_back(k2);
                        m_indices.push_back(k1 + 1);
                    }

                    if (i != (stack_count - 1)) {
                        m_indices.push_back(k1 + 1);
                        m_indices.push_back(k2);
                        m_indices.push_back(k2 + 1);
                    }
                }
            }

            // Create a unique vector with all data
            std::size_t count = m_vertices.size();
            for (std::size_t i = 0; i < count; i += 3) {
                m_sphereData.push_back(m_vertices[i]);
                m_sphereData.push_back(m_vertices[i + 1]);
                m_sphereData.push_back(m_vertices[i + 2]);

                m_sphereData.push_back(m_normals[i]);
                m_sphereData.push_back(m_normals[i + 1]);
                m_sphereData.push_back(m_normals[i + 2]);
            }

            // Load data to GPU
            p_shader->Use();

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &iVBO);
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, m_sphereData.size() * sizeof(float), &m_sphereData[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iVBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(float), &m_indices[0], GL_STATIC_DRAW); 

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
        }

        unsigned int GetSphereVAO() { return VAO; }
        unsigned int GetSphereVBO() { return VBO; }
        unsigned int GetSphereiVBO() { return iVBO; }
        unsigned int GetNumOfIndices() { return m_indices.size(); }
        unsigned int VAO, VBO, iVBO;
    private:
        std::vector<float> m_vertices;
        std::vector<float> m_normals;
        std::vector<float> m_sphereData;
        std::vector<unsigned int> m_indices; // Probably not needed
    };
}

#endif
