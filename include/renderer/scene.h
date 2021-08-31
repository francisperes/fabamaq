#ifndef SCENE_H
#define SCENE_H

#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h> // rand()
#include <time.h> // time()
#include "camera.h"
#include "shader.h"
#include "sphere.h"
#include "light.h"

#define MAX_ENTITIES 160

namespace renderer {
    class Scene {
    public:
        Scene(float aspect_ratio);
        ~Scene();

        void Render(std::vector<glm::vec3> positions);
        void Update(float delta);
        glm::vec3 GetColourFromIndex(int i) { return m_colours[i]; } // This should return an ENUM to make if statements more redable
        Camera* GetCameraPointer() { return &m_camera; }

    private:
        void LoadPositionBuffers();
        void LoadColourBuffers();
        void UpdateColours();
        // Only one sphere will be required and we can call it sphereModel
        // since we will be using instanced data we can the request all
        // positions from physics::World and send it all to the GPU
        Shader m_shader;
        Camera m_camera;
        Light  m_light;
        Sphere m_sphereModel;

        unsigned int m_instanceVBO;
        unsigned int m_colourVBO;
        float m_timer;
        glm::mat4 m_instanceData[MAX_ENTITIES];
        glm::vec3 m_colours[MAX_ENTITIES];
    };

}

#endif
