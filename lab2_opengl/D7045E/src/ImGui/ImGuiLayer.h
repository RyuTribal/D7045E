#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace D7045E {

	class ImGuiLayer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		void OnAttach();
		void OnDetach();
		void OnEvent(Event& e);

		void Begin();
		void End();

		void OnImGuiRender();

		void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		void RenderDockSpace();
	private:
		bool m_BlockEvents = true;
	};

}
