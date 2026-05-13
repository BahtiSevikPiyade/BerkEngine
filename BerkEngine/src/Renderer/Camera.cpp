#include "bepch.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace BerkEngine {

Camera::Camera(float fov, float aspectRatio, float near, float far)
    : m_FOV(fov),
      m_AspectRatio(aspectRatio),
      m_Near(near),
      m_Far(far),
      m_Yaw(-90.0f),
      m_Pitch(0.0f),
      m_Position(0.0f, 0.0f, 2.0f),
      m_UpVector(0.0f, 1.0f, 0.0f)
{
    UpdateProjectionMatrix();
    UpdateDirectionVectors();
    UpdateViewMatrix();
}

void Camera::SetPosition(const glm::vec3& position)
{
    m_Position = position;
    UpdateViewMatrix();
}

void Camera::LookAt(const glm::vec3& target)
{
    m_LookDirection = glm::normalize(target - m_Position);
    UpdateViewMatrix();
}

void Camera::Move(const glm::vec3& offset)
{
    m_Position += m_RightVector * offset.x;
    m_Position += m_UpVector * offset.y;
    m_Position += m_LookDirection * offset.z;
    UpdateViewMatrix();
}

void Camera::Rotate(float yaw, float pitch)
{
    m_Yaw += yaw;
    m_Pitch += pitch;

    // Pitch'i -89 ile 89 derece arasında tut
    if (m_Pitch > 89.0f)
        m_Pitch = 89.0f;
    if (m_Pitch < -89.0f)
        m_Pitch = -89.0f;

    UpdateDirectionVectors();
    UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_LookDirection, m_UpVector);
}

void Camera::UpdateProjectionMatrix()
{
    m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
}

void Camera::UpdateDirectionVectors()
{
    // Yaw ve Pitch'ten look direction hesapla
    glm::vec3 direction;
    direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    direction.y = sin(glm::radians(m_Pitch));
    direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_LookDirection = glm::normalize(direction);

    // Right vector hesapla
    m_RightVector = glm::normalize(glm::cross(m_LookDirection, glm::vec3(0.0f, 1.0f, 0.0f)));

    // Up vector hesapla
    m_UpVector = glm::normalize(glm::cross(m_RightVector, m_LookDirection));
}

void Camera::SetPerspective(float fov, float aspectRatio, float near, float far)
{
    m_FOV = fov;
    m_AspectRatio = aspectRatio;
    m_Near = near;
    m_Far = far;
    UpdateProjectionMatrix();
}

} // namespace BerkEngine
