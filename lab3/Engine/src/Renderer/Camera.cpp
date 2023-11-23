#include "pch.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {
	Camera::Camera(float left, float right, float bottom, float top, float camera_near, float camera_far)
	{
		SetOrthographic(left, right, bottom, top, camera_near, camera_far);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	Camera::Camera(float fovy, float aspect, float camera_near, float camera_far)
	{
		SetPerspective(fovy, aspect, camera_near, camera_far);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void Camera::SetOrthographic(float left, float right, float bottom, float top, float camera_near, float camera_far)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, camera_near, camera_far);
		m_Type = CameraType::ORTHOGRAPHIC;
		RecalculateViewMatrix();
	}
	void Camera::SetPerspective(float fovy, float aspect, float camera_near, float camera_far)
	{
		m_ProjectionMatrix = glm::perspective(fovy, aspect, camera_near, camera_far);
		m_Type = CameraType::PERSPECTIVE;
		RecalculateViewMatrix();
	}
	void Camera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), m_Position);

		transform = glm::rotate(transform, m_Rotation, glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
