#include <Engine.h>
#include <Core/EntryPoint.h>

#include "ImGuiBeginLayer.h"
#include "RandomObjectsLayer.h"
#include "CameraLayer.h"
#include "ImGuiEndLayer.h"

class App : public Engine::Application
{
public:
	App(Engine::WindowProps props) : Application(props), scene(Engine::CreateRef<Engine::Scene>("Lab4 scene"))
	{
		Engine::Renderer::Get()->SetBackgroundColor(0, 0, 0);

		Engine::Ref<Engine::Entity> camera_entity = scene->CreateEntity("Main Camera", nullptr);
		Engine::Ref<Engine::Camera> camera = Engine::CreateRef<Engine::Camera>(Engine::CameraType::PERSPECTIVE);
		camera_entity->AddComponent<Engine::CameraComponent>(Engine::CameraComponent(camera));
		camera_entity->AddComponent<Engine::LocalTransformComponent>(Engine::LocalTransformComponent(glm::vec3(0.f, 0.f, 0.f)));

		scene->SetCurrentCamera(camera);

		PushLayer(new Lab4::ImGuiBeginLayer(scene));
		PushLayer(new Lab4::RandomObjectsLayer(scene, camera_entity));
		PushLayer(new Lab4::CameraLayer(scene, camera_entity));
		PushLayer(new Lab4::ImGuiEndLayer());
	}

	~App()
	{

	}

private:
	Engine::Ref<Engine::Scene> scene;
};

Engine::Application* Engine::CreateApplication()
{
	WindowProps props{};
	props.Title = "Lab 4";
	props.Width = 1280;
	props.Height = 720;
	return new App(props);
}