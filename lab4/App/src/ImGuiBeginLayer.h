#pragma once
#include <Engine.h>
#include <imgui/imgui.h>

namespace Lab4 {
	class ImGuiBeginLayer : public Engine::Layer {
	public:
		ImGuiBeginLayer(Engine::Ref<Engine::Scene> scene) : Layer("ImGuiBegin Layer"), scene(scene) {
		
		}
		~ImGuiBeginLayer() = default;

		void OnUpdate(float delta_time) override {
			Engine::Renderer::Get()->BeginFrame(scene->GetCurrentCamera());
		}

		void OnImGuiRender() override {
			ImGui::Begin("Settings");
		}

	private:
		Engine::Ref<Engine::Scene> scene;
	};
}