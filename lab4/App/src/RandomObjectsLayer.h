#pragma once
#include <Engine.h>
#include <imgui/imgui.h>

namespace Lab4 {

	struct MazeNode {
		bool left_wall;
		bool right_wall;
		bool up_wall;
		bool down_wall;

		MazeNode(bool left, bool right, bool up, bool down) : left_wall(left), right_wall(right), up_wall(up), down_wall(down){}
	};

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
		void CreateWall(Engine::TransformComponent new_transform, Engine::Entity* maze_entity, const std::string& name);
		void CreateRobot();

	private:

		Engine::Ref<Engine::Scene> scene;
		std::unordered_map <Engine::UUID, Engine::Ref<Engine::Entity>> entities{};

		Engine::UUID child_tree_node_id = Engine::UUID();

		Engine::Component* component_selected = nullptr;
		Engine::UUID entity_selected = Engine::UUID();

		std::unordered_map<Engine::UUID, std::unordered_map<Engine::ComponentType, bool>> selected_option;
		std::pair<Engine::UUID, Engine::ComponentType> currently_selected;

		std::unordered_map<std::string, Engine::TextureInfo> texture_library;

		Engine::Ref<Engine::Entity> m_HeadEntity;
		Engine::Ref<Engine::Entity> m_StarEntity;
		float animationTime = 0.f;
	};
}