#pragma once
#include <Engine.h>
#include <map>

namespace Lab3 {
	class MovableObjectLayer : public Engine::Layer {
	public:
		MovableObjectLayer() : Layer("MovableObject") {}
		~MovableObjectLayer() = default;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float delta_time) override;
		void OnEvent(Engine::Event& event) override;
		void OnImGuiRender() override;

	private:
		bool OnKeyPress(Engine::KeyPressedEvent& event);
		bool OnKeyRelease(Engine::KeyReleasedEvent& event);

		void UpdateMovement();

		void MoveObject(glm::vec3 velocity);

		void UpdateKeyState(int keyCode, bool isPressed);
	private:
		Engine::Ref<Engine::GraphicsNode> movable_object;
		int BG[3] = { 128, 128, 128 };

		const float MAX_FRAME_TIME = 0.01667;

		float speed = 1.0f;

		float smoothedDeltaTime = 0.0f;
		const float smoothingFactor = 0.9f;

		std::map<int, bool> keyStates;
	};
}