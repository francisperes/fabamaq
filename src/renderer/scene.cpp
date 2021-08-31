#include "renderer/scene.h"

namespace renderer {
    Scene::Scene(float aspect_ratio) :
        m_shader{Shader("resources/shaders/vs.glsl", "resources/shaders/fs.glsl")},
        m_camera{Camera(glm::vec3(0.0f, 0.0f, 15.0f), aspect_ratio, 0.1f, 100.0f)},
        m_light{Light(glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f)), glm::vec3(1.0f, 1.0f, 1.0f))},
        m_sphereModel{Sphere(&m_shader, 0.2f, 32, 18)}
    {
        // Init rand
        srand(time(NULL));
        // Configure instance buffer
        m_shader.Use();
        glGenBuffers(1, &m_instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * MAX_ENTITIES, &m_instanceData[0][0], GL_STATIC_DRAW);

        // Since the biggest datatype allowed in vertex attributes is vec4 we need to create
        // 4 points, since mat4 = 4 * vec4
        glBindVertexArray(m_sphereModel.VAO);
        LoadPositionBuffers();
        
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glGenBuffers(1, &m_colourVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_colourVBO);
        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * MAX_ENTITIES, &m_colours[0], GL_STATIC_DRAW);
        LoadColourBuffers();

        glVertexAttribDivisor(7, 1);

        glBindVertexArray(0);

        UpdateColours();
    }

    Scene::~Scene() {}

    void Scene::Render(std::vector<glm::vec3> positions) {
        m_shader.Use();
        glBindVertexArray(m_sphereModel.VAO);

        // Prepare data
        glm::mat4 projection = m_camera.GetProjectionMatrix();
        glm::mat4 view = m_camera.GetViewMatrix();
        glm::vec3 camera_pos = m_camera.GetPosition();

        for (int i = 0; i < MAX_ENTITIES; i++) {
            glm::mat4 m = glm::mat4(1.0f);
            m = glm::translate(m, positions[i]);
            m_instanceData[i] = m;
            // if (timer 
        }

        // Send data to shader
        m_shader.SetMat4("mProjection", projection);
        m_shader.SetMat4("mView", view);
        m_shader.SetVec3("viewPos", camera_pos);
        m_light.Render(&m_shader);


        // Load Model matrix
        // Since the biggest datatype allowed in vertex attributes is vec4 we need to create
        // 4 points, since mat4 = 4 * vec4
        LoadPositionBuffers();
        LoadColourBuffers();

        // Draw objects
        glBindBuffer(GL_ARRAY_BUFFER, m_sphereModel.GetSphereVBO());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphereModel.GetSphereiVBO());

        glDrawElementsInstanced(GL_TRIANGLES, m_sphereModel.GetNumOfIndices(), GL_UNSIGNED_INT, (void*)0, MAX_ENTITIES);
    }

    void Scene::Update(float delta) {
        m_timer += delta;

        if (m_timer > 0.1f) {
            UpdateColours();
            m_timer = 0.0f;
        }
    }

    void Scene::LoadPositionBuffers() {
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * MAX_ENTITIES, &m_instanceData[0][0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(0 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
    }

    void Scene::LoadColourBuffers() {
        glBindBuffer(GL_ARRAY_BUFFER, m_colourVBO);
        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * MAX_ENTITIES, &m_colours[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    void Scene::UpdateColours() {
        for (auto &colour : m_colours) {
            int n = rand() % 3;
            if (n == 0) { colour = glm::vec3(1.0f, 0.0f, 0.0f); }
            if (n == 1) { colour = glm::vec3(0.0f, 1.0f, 0.0f); }
            if (n == 2) { colour = glm::vec3(0.0f, 0.0f, 1.0f); }
        }
    }
}
