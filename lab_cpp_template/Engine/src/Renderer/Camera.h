#pragma once

#include <glm/glm.hpp>

namespace Engine {

	enum CameraType {
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	class Camera {
	public:
		
		Camera(CameraType type);

		const glm::vec3& GetPosition() const { return m_Position; }
		float GetRotation() const { return m_Rotation; }

		const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewProjection() const { return m_ViewProjectionMatrix; }
		const glm::mat4& GetView() const { return m_ViewMatrix; }
		const CameraType& GetType() const { return m_Type; }

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
		void SetOrthographicSize(float size);
		void SetClippingRange(float near, float far);
		/*
		* In degrees
		*/
		void SetFovy(float fovy);
		void SetAspectRatio(float ratio);

		void ChangeCameraType(CameraType type);
	private:
		void RecalculateViewMatrix();
		void SetOrthographic();
		void SetPerspective();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix{ 1.f };
		glm::mat4 m_ViewProjectionMatrix{ 1.f };

		glm::vec3 m_Position = glm::vec3(0.f, 0.f, 0.f);
		float m_Rotation = 0.0f;

		float m_OrthographicSize = 1.f;

		float m_Near = 0.1f;
		float m_Far = 1000.f;

		float m_PerspectiveFOVY = glm::radians(45.f);

		float m_AspectRatio = 1280.f / 720.f;


		CameraType m_Type;
	};


}