#include "renderer/camera.h"

namespace renderer {

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 15.0f;
const float MAX_PITCH = 89.8f;
const float MIN_PITCH = -89.8f;

Camera::Camera(glm::vec3 position, float aspect_ratio, float near_plane_distance, float far_plane_distance) {
    m_position = position;
    m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_yaw = YAW;
    m_pitch = PITCH;
    m_near = near_plane_distance;
    m_far = far_plane_distance;
    m_ratio = aspect_ratio;
    m_positionChanged = false;

    UpdateCameraVectors();
}

Camera::~Camera() {}

//void Camera::ProcessInput(const Uint8* state) { /* TODO */}

void Camera::Update(float delta) {
    if (!m_positionChanged) { return; }
    
    m_position += m_right * m_move.x * SPEED * delta;
    m_position += m_up * m_move.y * SPEED * delta;
    m_position += m_front * m_move.z * SPEED * delta;

    if (m_pitch > MAX_PITCH) { m_pitch = MAX_PITCH; }
    if (m_pitch < MIN_PITCH) { m_pitch = MIN_PITCH; }

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    new_front.y = sin(glm::radians(m_pitch));
    new_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(new_front);
    // Recalculate Right and Up vectors
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::GetProjectionMatrix() {
    return glm::perspective(glm::radians(45.0f), m_ratio, m_near, m_far);
}

glm::vec3 Camera::GetPosition() { return m_position; }

}
