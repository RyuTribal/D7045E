#include <Engine.h>
#include <Core/EntryPoint.h>

#include "Lab2Layer.h"

class Lab2 : public Engine::Application
{
public:
	Lab2()
	{
		PushLayer(new Lab2Layer());
	}

	~Lab2()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Lab2();
}