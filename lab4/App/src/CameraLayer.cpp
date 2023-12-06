#include "CameraLayer.h"
#include <imgui/imgui.h>

namespace Lab4 {
	void CameraLayer::OnAttach()
	{
	}
	void CameraLayer::OnDetach()
	{
	}
	void CameraLayer::OnUpdate(float delta_time)
	{
		smoothedDeltaTime = smoothingFactor * smoothedDeltaTime + (1.0f - smoothingFactor) * delta_time;
		UpdateMovement();
		if (!HasMovement()) {
			ApplyFriction();
		}

		scene->UpdateScene(delta_time);
	}
	void CameraLayer::OnEvent(Engine::Event& event)
	{
		Engine::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<Engine::KeyPressedEvent>(BIND_EVENT_FN(Lab4::CameraLayer::OnKeyPress));
		dispatcher.Dispatch<Engine::KeyReleasedEvent>(BIND_EVENT_FN(Lab4::CameraLayer::OnKeyRelease));
		dispatcher.Dispatch<Engine::MouseButtonPressedEvent>(BIND_EVENT_FN(Lab4::CameraLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<Engine::MouseButtonReleasedEvent>(BIND_EVENT_FN(Lab4::CameraLayer::OnMouseButtonReleased));
		dispatcher.Dispatch<Engine::MouseMovedEvent>(BIND_EVENT_FN(Lab4::CameraLayer::OnMouseMoved));
	}
	void CameraLayer::OnImGuiRender()
	{
	}

	void CameraLayer::SetDelta() {
		if (firstClick) {
			currentMouseOrientation.x = Engine::Input::GetMouseX();
			currentMouseOrientation.y = Engine::Input::GetMouseY();
			firstClick = false;
		}

		deltaMouseOrientation = glm::vec2(0.f, 0.f);
		deltaMouseOrientation.x = Engine::Input::GetMouseX();
		deltaMouseOrientation.y = Engine::Input::GetMouseY();
		deltaMouseOrientation = currentMouseOrientation - deltaMouseOrientation;
		currentMouseOrientation = { 0.f, 0.f };
		currentMouseOrientation.x = Engine::Input::GetMouseX();
		currentMouseOrientation.y = Engine::Input::GetMouseY();
	}

	bool CameraLayer::HasMovement()
	{
		if (keyStates[KEY_W]) {
			return true;
		}
		else if (keyStates[KEY_S]) {
			return true;
		}
		else if (keyStates[KEY_A]) {
			return true;
		}
		else if (keyStates[KEY_D]) {
			return true;
		}
		return false;
	}


	bool CameraLayer::OnKeyPress(Engine::KeyPressedEvent& event)
	{
		UpdateKeyState(event.GetKeyCode(), true);
		if (HasMovement()) {
			BuildVelocityVector();
		}
		return true;
	}
	bool CameraLayer::OnKeyRelease(Engine::KeyReleasedEvent& event)
	{
		UpdateKeyState(event.GetKeyCode(), false);
		return true;
	}
	bool CameraLayer::OnMouseButtonReleased(Engine::MouseButtonReleasedEvent& event)
	{
		if (event.GetMouseButton() == MOUSE_BUTTON_RIGHT) {
			mouseIsPressed = false;
			Engine::Input::SetLockMouseMode(false);
		}
		return true;
	}
	bool CameraLayer::OnMouseMoved(Engine::MouseMovedEvent& event)
	{
		if (mouseIsPressed) {
			SetDelta();
		}
		return true;
	}

	void CameraLayer::BuildVelocityVector()
	{
		auto camera = scene->GetCurrentCamera();
		velocity = glm::vec3(0.0f);

		if (keyStates[KEY_W]) {
			velocity += mouseIsPressed ? camera->GetForwardDirection() * speed : glm::vec3(0.0f, speed, 0.0f);
		}
		if (keyStates[KEY_S]) {
			velocity += mouseIsPressed ? -(camera->GetForwardDirection() * speed) : glm::vec3(0.0f, -speed, 0.0f);
		}
		if (keyStates[KEY_A]) {
			velocity += mouseIsPressed ? -(camera->GetRightDirection() * speed) : glm::vec3(-speed, 0.0f, 0.0f);
		}
		if (keyStates[KEY_D]) {
			velocity += mouseIsPressed ? camera->GetRightDirection() * speed : glm::vec3(speed, 0.0f, 0.0f);
		}
	}
	
	bool CameraLayer::OnMouseButtonPressed(Engine::MouseButtonPressedEvent& event)
	{
		if (event.GetMouseButton() == MOUSE_BUTTON_RIGHT) {
			mouseIsPressed = true;
			firstClick = true;
			Engine::Input::SetLockMouseMode(true);
		}
		return true;
	}


	void CameraLayer::UpdateKeyState(int keyCode, bool isPressed) {
		keyStates[keyCode] = isPressed;
	}

	void CameraLayer::UpdateMovement() {
		auto camera = scene->GetCurrentCamera();

		if (mouseIsPressed) {
			camera->Rotate(deltaMouseOrientation*smoothedDeltaTime, sensitivity, false);
			deltaMouseOrientation = { 0.f, 0.f };
		}

		if (velocity != glm::vec3(0.0f)) {
			camera->Move(velocity * smoothedDeltaTime);
		}
	}
	void CameraLayer::ApplyFriction() {
		velocity *= 1.0 - air_friction;

		if (glm::length(velocity) < 0.01f) {
			velocity = glm::vec3(0.0f);
		}
	}
}