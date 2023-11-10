#include "pch.h"

#include "Core/Application.h"
#include "Core/GlobalState.h"

int main(int argc, char** argv)
{
	D7045E::Log::Init();
	CORE_WARN("Log Initialized");

	auto app = new D7045E::Application();
	app->run();
	delete app;
}
