#pragma once

#include <Engine.h>
#include <imgui/imgui.h>

namespace Lab4 {
	class ImGuiEndLayer : public Engine::Layer {
	public:
		ImGuiEndLayer() : Layer("ImGuiEnd Layer") {}
		~ImGuiEndLayer() = default;

		void OnUpdate(float delta_time) {
			Engine::Renderer::Get()->EndFrame();
		}

		void OnImGuiRender() override {
			ImGui::End();
		}
	};
}