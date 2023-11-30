#pragma once
#include <Engine.h>
#include <imgui/imgui.h>

namespace Lab4 {

	class RandomObjectsLayer : public Engine::Layer {
	public:
		RandomObjectsLayer(Engine::Ref<Engine::Scene> scene, Engine::Ref<Engine::Entity> camera_entity) : Layer("RandomObjects"), scene(scene) {
			entities[camera_entity->GetID()] = camera_entity;
		}
		~RandomObjectsLayer() = default;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float delta_time) override;
		void OnEvent(Engine::Event& event) override;
		void OnImGuiRender() override;

	private:
		void DisplaySceneNode(Engine::SceneNode* node);
		void DisplayComponents(Engine::UUID entity_id);
		void DisplayDetails();

	private:

		Engine::Ref<Engine::Scene> scene;
		std::unordered_map <Engine::UUID, Engine::Ref<Engine::Entity>> entities{};
		bool isOrthographic = false;

		const int random_object_count = 7;

		ImVec4 plusColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		ImVec4 minusColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);

		Engine::UUID child_tree_node_id = Engine::UUID();

		Engine::Component* component_selected = nullptr;
		Engine::UUID entity_selected = Engine::UUID();

		std::unordered_map<Engine::UUID, std::unordered_map<Engine::ComponentType, bool>> selected_option;
		std::pair<Engine::UUID, Engine::ComponentType> currently_selected;

		std::unordered_map<std::string, Engine::TextureInfo> texture_library;
	};
}