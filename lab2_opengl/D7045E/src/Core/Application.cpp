#include "pch.h"
#include "Application.h"

#include "Renderer.h"
#include "glad/gl.h"

namespace D7045E
{
	Application* Application::s_Instance = nullptr;

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();

		m_ImGuiLayer->OnAttach();

		Renderer::CreateRenderer("shaders/simple_shader.vert", "shaders/simple_shader.frag");
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));

		
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		auto last_frame = std::chrono::high_resolution_clock::now();
		while (m_Running)
		{
			auto width = GetWindow().GetWidth();
			auto height = GetWindow().GetHeight();
			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - last_frame).count();
			last_frame = newTime;
			
			Renderer::Get()->PrepareFrame(frameTime);

			glViewport(0, 0, width - (width / 3), height);

			Renderer::Get()->Draw();

			glViewport(0, 0, width, height);

			m_ImGuiLayer->Begin();

			m_ImGuiLayer->OnImGuiRender();

			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return false;
	}
}
