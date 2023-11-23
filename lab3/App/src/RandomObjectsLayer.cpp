#include "RandomObjectsLayer.h"
#include <imgui/imgui.h>

namespace Lab3 {
	void RandomObjectsLayer::OnAttach()
	{
		CORE_WARN("Random objects layer attached");

		Engine::Ref<Engine::Mesh> mesh = Engine::CreateRef<Engine::Cuboid>(0.5f, 0.5f, 0.5f);

		Engine::Ref<Engine::Material> material = Engine::CreateRef<Engine::Monochrome>(255, 0, 0);

		Engine::Ref<Engine::GraphicsNode> node = Engine::CreateRef<Engine::GraphicsNode>(mesh, material, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f));

		objects.push_back(node);
	}
	void RandomObjectsLayer::OnDetach()
	{
	}
	void RandomObjectsLayer::OnUpdate(float delta_time)
	{
		Engine::Renderer::Get()->BeginFrame(BG);

		for (Engine::Ref<Engine::GraphicsNode> node : objects) {
			Engine::Renderer::Get()->Draw(node);
		}
		Engine::Renderer::Get()->EndFrame();
	}
	void RandomObjectsLayer::OnEvent(Engine::Event& event)
	{
	}
	void RandomObjectsLayer::OnImGuiRender()
	{
		ImGui::ShowDemoWindow();
	}
}