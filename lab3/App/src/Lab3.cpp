#include <Engine.h>
#include <Core/EntryPoint.h>

#include "RandomObjectsLayer.h"
#include "MovableObjectLayer.h"

class App : public Engine::Application
{
public:
	App(Engine::WindowProps props) : Application(props)
	{
		Engine::Renderer::Get()->SetBackgroundColor(128, 128, 128);
		PushLayer(new Lab3::RandomObjectsLayer());
		PushLayer(new Lab3::MovableObjectLayer());
	}

	~App()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	WindowProps props{};
	props.Title = "Lab3";
	props.Width = 1280;
	props.Height = 720;
	return new App(props);
}