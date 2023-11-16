#include "pch.h"
#include "Application.h"

#include "Renderer.h"
#include "glad/gl.h"

namespace Engine
{
	Application* Application::s_Instance = nullptr;

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::CreateRenderer();

		m_ImGuiLayer = new ImGuiLayer();

		PushOverlay(m_ImGuiLayer);
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushLayer(overlay);
		overlay->OnAttach();
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

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(frameTime);
			}
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
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
