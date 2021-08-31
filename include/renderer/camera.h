#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <SDL2/SDL_scancode.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace renderer {

class Camera {
public:
    Camera(glm::vec3 position, float aspect_ratio, float near_plane_distance, float far_plane_distance);
    ~Camera();
    
    //void ProcessInput(const Uint8* state);
    void Update(float delta);
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    glm::vec3 GetPosition();
    //glm::vec3 GetFront();
    //float GetPitch();
    //float GetYaw();

private:
    void UpdateCameraVectors();

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;
    glm::vec3 m_move;
    //glm::vec3 m_rotation; // Not sure if needed
    bool m_positionChanged;
    // Euler angles
    float m_yaw;
    float m_pitch;
    // Prespective matrix data
    float m_near;
    float m_far;
    float m_ratio;
};

}

#endif
