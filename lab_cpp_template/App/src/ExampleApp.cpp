#include <Engine.h>
#include <Core/EntryPoint.h>

class ExampleApp : public Engine::Application
{
public:
	ExampleApp()
	{
	}

	~ExampleApp()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new ExampleApp();
}