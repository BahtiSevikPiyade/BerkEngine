#pragma once

#include <glm/glm.hpp>

namespace BerkEngine {

class Camera {
public:
    Camera(float fov = 45.0f, float aspectRatio = 16.0f / 9.0f, float near = 0.1f, float far = 100.0f);

    // Getters
    glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
    glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
    glm::mat4 GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }

    glm::vec3 GetPosition() const { return m_Position; }
    glm::vec3 GetLookDirection() const { return m_LookDirection; }
    glm::vec3 GetUpVector() const { return m_UpVector; }

    // Position and rotation control
    void SetPosition(const glm::vec3& position);
    void LookAt(const glm::vec3& target);
    void UpdateViewMatrix();

    // Modifier methods
    void Move(const glm::vec3& offset);
    void Rotate(float yaw, float pitch);

    // Perspective control
    void SetPerspective(float fov, float aspectRatio, float near, float far);

private:
    glm::vec3 m_Position;
    glm::vec3 m_LookDirection;
    glm::vec3 m_UpVector;
    glm::vec3 m_RightVector;

    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjectionMatrix;

    float m_FOV;
    float m_AspectRatio;
    float m_Near;
    float m_Far;
    float m_Yaw;
    float m_Pitch;

    void UpdateProjectionMatrix();
    void UpdateDirectionVectors();
};

} // namespace BerkEngine
