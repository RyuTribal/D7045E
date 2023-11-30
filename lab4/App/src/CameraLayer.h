#pragma once
#include <Engine.h>
#include <map>

namespace Lab4 {
	class CameraLayer : public Engine::Layer {
	public:
		CameraLayer(Engine::Ref<Engine::Scene> scene, Engine::Ref<Engine::Entity> camera_entity) : Layer("Camera Layer"), scene(scene), camera_entity(camera_entity) {}
		~CameraLayer() = default;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float delta_time) override;
		void OnEvent(Engine::Event& event) override;
		void OnImGuiRender() override;

	private:
		void SetDelta();

		bool OnKeyPress(Engine::KeyPressedEvent& event);
		bool OnKeyRelease(Engine::KeyReleasedEvent& event);
		bool OnMouseButtonPressed(Engine::MouseButtonPressedEvent& event);
		bool OnMouseButtonReleased(Engine::MouseButtonReleasedEvent& event);
		bool OnMouseMoved(Engine::MouseMovedEvent& event);

		void BuildVelocityVector();

		bool HasMovement();

		void UpdateMovement();
		void ApplyFriction();

		void UpdateKeyState(int keyCode, bool isPressed);
	private:
		Engine::Ref<Engine::Scene> scene;
		Engine::Ref<Engine::Entity> camera_entity;
		int BG[3] = { 128, 128, 128 };

		const float MAX_FRAME_TIME = 0.01667;

		float speed = 1.0f;

		glm::vec3 velocity = { 0.f, 0.f, 0.f };

		float air_friction = 0.1f;

		float sensitivity = 0.1f;

		float smoothedDeltaTime = 0.0f;
		const float smoothingFactor = 0.9f;

		std::map<int, bool> keyStates;
		bool mouseIsPressed = false;
		bool firstClick = true;

		glm::vec2 deltaMouseOrientation = { 0.f, 0.f };
		glm::vec2 currentMouseOrientation = { 0.f, 0.f };
	};
}