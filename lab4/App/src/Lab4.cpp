#include <Engine.h>
#include <Core/EntryPoint.h>

#include "ImGuiBeginLayer.h"
#include "RandomObjectsLayer.h"
#include "CameraLayer.h"
#include "ImGuiEndLayer.h"

class App : public Engine::Application
{
public:
	App(Engine::WindowProps props) : Application(props)
	{
		auto [camera_entity, scene] = Engine::Scene::CreateScene("Lab4 scene");
		Engine::Renderer::Get()->SetBackgroundColor(128, 128, 128);

		PushLayer(new Lab4::ImGuiBeginLayer(scene));
		PushLayer(new Lab4::RandomObjectsLayer(scene, camera_entity));
		PushLayer(new Lab4::CameraLayer(scene, camera_entity));
		PushLayer(new Lab4::ImGuiEndLayer());
	}

	~App()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	WindowProps props{};
	props.Title = "Lab 4";
	props.Width = 1280;
	props.Height = 720;
	return new App(props);
}