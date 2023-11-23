#pragma once

#include <glm/glm.hpp>

namespace Engine {

	enum CameraType {
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	class Camera {
	public:
		
		Camera(float fovy, float aspect, float camera_near, float camera_far); // Start as Perspective
		Camera(float left, float right, float bottom, float top, float camera_near, float camera_far); // Start as Orthographic

		const glm::vec3& GetPosition() const { return m_Position; }
		float GetRotation() const { return m_Rotation; }

		const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewProjection() const { return m_ViewProjectionMatrix; }
		const glm::mat4& GetView() const { return m_ViewMatrix; }
		const CameraType& GetType() const { return m_Type; }

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		void SetOrthographic(float left, float right, float bottom, float top, float camera_near, float camera_far);
		void SetPerspective(float fovy, float aspect, float camera_near, float camera_far);
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix{ 1.f };
		glm::mat4 m_ViewProjectionMatrix{ 1.f };

		glm::vec3 m_Position = glm::vec3(0.f, 0.f, 0.f);
		float m_Rotation = 0.0f;

		CameraType m_Type = CameraType::PERSPECTIVE;
	};


}