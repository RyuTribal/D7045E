#include "MovableObjectLayer.h"
#include <imgui/imgui.h>

namespace Lab3 {
	void MovableObjectLayer::OnAttach()
	{
		Engine::Ref<Engine::Mesh> mesh = Engine::CreateRef<Engine::Cuboid>(0.5f, 0.5f, 0.5f);

		Engine::Ref<Engine::Material> material = Engine::CreateRef<Engine::Monochrome>(0, 255, 0);

		movable_object = Engine::CreateRef<Engine::GraphicsNode>(mesh, material, glm::vec3(0.f, 0.f, -2.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f));
	}
	void MovableObjectLayer::OnDetach()
	{
	}
	void MovableObjectLayer::OnUpdate(float delta_time)
	{
		smoothedDeltaTime = smoothingFactor * smoothedDeltaTime + (1.0f - smoothingFactor) * delta_time;
		UpdateMovement();
		Engine::Renderer::Get()->Draw(movable_object);
	}
	void MovableObjectLayer::OnEvent(Engine::Event& event)
	{
		Engine::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<Engine::KeyPressedEvent>(BIND_EVENT_FN(Lab3::MovableObjectLayer::OnKeyPress));
		dispatcher.Dispatch<Engine::KeyReleasedEvent>(BIND_EVENT_FN(Lab3::MovableObjectLayer::OnKeyRelease));
	}
	void MovableObjectLayer::OnImGuiRender()
	{
	}
	bool MovableObjectLayer::OnKeyPress(Engine::KeyPressedEvent& event)
	{
		UpdateKeyState(event.GetKeyCode(), true);
		return true;
	}
	bool MovableObjectLayer::OnKeyRelease(Engine::KeyReleasedEvent& event)
	{
		UpdateKeyState(event.GetKeyCode(), false);
		return true;
	}
	void MovableObjectLayer::MoveObject(glm::vec3 velocity)
	{
		movable_object->Move(velocity * smoothedDeltaTime);
	}


	void MovableObjectLayer::UpdateKeyState(int keyCode, bool isPressed) {
		keyStates[keyCode] = isPressed;
	}

	void MovableObjectLayer::UpdateMovement() {
		glm::vec3 movement(0.0f);

		if (keyStates[KEY_W]) {
			movement.y += speed;
		}
		if (keyStates[KEY_A]) {
			movement.x -= speed;
		}
		if (keyStates[KEY_S]) {
			movement.y -= speed;
		}
		if (keyStates[KEY_D]) {
			movement.x += speed;
		}
		if (keyStates[KEY_E]) {
			movement.z -= speed;
		}
		if (keyStates[KEY_C]) {
			movement.z += speed;
		}

		if (movement != glm::vec3(0.0f)) {
			MoveObject(movement);
		}
	}
}