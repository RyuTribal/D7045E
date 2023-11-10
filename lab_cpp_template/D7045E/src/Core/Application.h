#pragma once
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"

namespace D7045E {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		void OnEvent(Event& event);
		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
