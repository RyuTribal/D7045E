#include "RandomObjectsLayer.h"
#include <imgui/imgui.h>

namespace Lab3 {
	void RandomObjectsLayer::OnAttach()
	{
		CORE_WARN("Random objects layer attached");

		Engine::Ref<Engine::Mesh> mesh = Engine::CreateRef<Engine::Cuboid>(0.5f, 0.5f, 0.5f);

		Engine::Ref<Engine::Material> material = Engine::CreateRef<Engine::Monochrome>(255, 0, 0);

		for (int i = 0; i < random_object_count; i++) {
			float random_x = Engine::Random::RandomFloat(-0.7f, 0.7f);
			float random_y = Engine::Random::RandomFloat(-0.7f, 0.7f);
			float random_z = Engine::Random::RandomFloat(-3.f, -1.f); // so that it doesnt clip the camera since camera is at origin

			float random_scale = Engine::Random::RandomFloat(0.f, 0.5f);

			Engine::Ref<Engine::GraphicsNode> node = Engine::CreateRef<Engine::GraphicsNode>(mesh, material, glm::vec3(random_x, random_y, random_z), glm::vec3(random_scale, random_scale, random_scale), glm::vec3(0.f, 0.f, 0.f));
			objects.push_back(node);
		}
	}
	void RandomObjectsLayer::OnDetach()
	{
	}
	void RandomObjectsLayer::OnUpdate(float delta_time)
	{

		for (Engine::Ref<Engine::GraphicsNode> node : objects) {
			Engine::Renderer::Get()->Draw(node);
		}
	}
	void RandomObjectsLayer::OnEvent(Engine::Event& event)
	{
	}
	void RandomObjectsLayer::OnImGuiRender()
	{
		ImGui::Begin("Settings");

		if (ImGui::Checkbox("Set Orthographic", &isOrthographic)) {
			if (isOrthographic) {
				Engine::Renderer::Get()->GetCamera().ChangeCameraType(Engine::CameraType::ORTHOGRAPHIC);
			}
			else {
				Engine::Renderer::Get()->GetCamera().ChangeCameraType(Engine::CameraType::PERSPECTIVE);
			}
		}
		
		ImGui::End();
	}
}