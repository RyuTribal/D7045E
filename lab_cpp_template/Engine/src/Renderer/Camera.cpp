#include "pch.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {
	Camera::Camera(CameraType type) : m_Type(type)
	{
		if (m_Type == CameraType::ORTHOGRAPHIC) {
			SetOrthographic();
		}
		else if (m_Type == CameraType::PERSPECTIVE) {
			SetPerspective();
		}

		RecalculateViewMatrix();
	}
	void Camera::SetOrthographicSize(float size)
	{
		m_OrthographicSize = size;
		if (m_Type == CameraType::ORTHOGRAPHIC) {
			SetOrthographic();

			RecalculateViewMatrix();
		}
	}
	void Camera::SetClippingRange(float near, float far)
	{
		m_Near = near; 
		m_Far = far; 
		if (m_Type == CameraType::PERSPECTIVE) {
			SetPerspective();
		}
		else if (m_Type == CameraType::ORTHOGRAPHIC) {
			SetOrthographic();
		}
		RecalculateViewMatrix();
	}
	void Camera::SetFovy(float fovy)
	{
		m_PerspectiveFOVY = glm::radians(fovy);

		if (m_Type == CameraType::PERSPECTIVE) {
			SetPerspective();
			RecalculateViewMatrix();
		}
	}
	void Camera::SetAspectRatio(float ratio)
	{
		m_AspectRatio = ratio;

		if (m_Type == CameraType::PERSPECTIVE) {
			SetPerspective();
			RecalculateViewMatrix();
		}
		else if (m_Type == CameraType::ORTHOGRAPHIC) {
			SetOrthographic();
			RecalculateViewMatrix();
		}
	}
	void Camera::ChangeCameraType(CameraType type)
	{
		m_Type = type;
		m_Type == CameraType::ORTHOGRAPHIC ? SetOrthographic() : SetPerspective();
		RecalculateViewMatrix();
	}
	void Camera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), m_Position);

		transform = glm::rotate(transform, m_Rotation, glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void Camera::SetOrthographic()
	{
		float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoBottom = -m_OrthographicSize * 0.5f;
		float orthoTop = m_OrthographicSize * 0.5f;
		m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_Near, m_Far);
	}
	void Camera::SetPerspective()
	{
		m_ProjectionMatrix = glm::perspective(m_PerspectiveFOVY, m_AspectRatio, m_Near, m_Far);
	}
}
