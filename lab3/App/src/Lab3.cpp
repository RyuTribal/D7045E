#include <Engine.h>
#include <Core/EntryPoint.h>

#include "RandomObjectsLayer.h"

class App : public Engine::Application
{
public:
	App()
	{
		PushLayer(new Lab3::RandomObjectsLayer());
	}

	~App()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new App();
}