#pragma once
#include <Engine.h>

namespace Lab3 {
	class RandomObjectsLayer : public Engine::Layer {
	public:
		RandomObjectsLayer() : Layer("RandomObjects") {}
		~RandomObjectsLayer() = default;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float delta_time) override;
		void OnEvent(Engine::Event& event) override;
		void OnImGuiRender() override;

	private:
		std::vector<Engine::Ref<Engine::GraphicsNode>> objects;
		int BG[3] = { 128, 128, 128 };
	};
}