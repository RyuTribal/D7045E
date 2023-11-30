#include "pch.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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
		if (type != m_Type) {
			m_Type = type;
			m_Type == CameraType::ORTHOGRAPHIC ? SetOrthographic() : SetPerspective();
			RecalculateViewMatrix();
		}
	}
	void Camera::RecalculateViewMatrix()
	{

		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();

		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);

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

	void Camera::Rotate(const glm::vec2& delta, float rotation_speed, bool inverse_controls)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		float deltaYaw = yawSign * delta.x * rotation_speed;
		float deltaPitch = delta.y * rotation_speed;

		if (!inverse_controls) {
			deltaYaw *= -1;
			deltaPitch *= -1;
		}

		m_Yaw += deltaYaw;
		m_Pitch += deltaPitch;
	}

	glm::vec3 Camera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 Camera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 Camera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 Camera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	glm::quat Camera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}
}
