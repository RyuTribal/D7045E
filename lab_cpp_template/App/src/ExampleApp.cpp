#include <Engine.h>
#include <Core/EntryPoint.h>

class ExampleApp : public Engine::Application
{
public:
	ExampleApp()
	{
	    auto [camera_entity, scene] = Engine::Scene::CreateScene("Test scene");
	    Engine::Renderer::Get()->SetBackgroundColor(128, 128, 128);
	    
	}

	~ExampleApp()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	WindowProps props{};
	props.Title = "Example App";
	props.Width = 1280;
	props.Height = 720;
	return new ExampleApp(props);
}
