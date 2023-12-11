#include "RandomObjectsLayer.h"
#include "Materials/Silver.h"
#include "Materials/Gold.h"
#include "Materials/Obsidian.h"
#include "Materials/Pearl.h"
#include "Primitives/Cuboid.h"
#include "Primitives/Sphere.h"
#include "Primitives/Cylinder.h"
#include "Primitives/Cone.h"
#include "Primitives/Torus.h"
#include "Primitives/ChristmasStar.h"
#include <imgui/imgui.h>
#include <filesystem>

namespace Lab4 {
	void RandomObjectsLayer::OnAttach()
	{
		CORE_WARN("Random objects layer attached");


		// Referenced some picture I found online
		std::vector<std::vector<MazeNode>> maze_layout = {
			{MazeNode(true, false, true, false), MazeNode(false, false, true, true), MazeNode(false, false, false, false), MazeNode(true, false, true, true), MazeNode(false, false, true, true), MazeNode(false, false, true, false), MazeNode(false, false, true, true), MazeNode(false, true, true, false)},
			{MazeNode(true, true, false, false), MazeNode(true, false, false, true), MazeNode(false, false, false, true), MazeNode(false, false, false, true), MazeNode(false, false, false, true), MazeNode(false, true, false, true), MazeNode(true, true, false, false), MazeNode(false, true, false, false)},
			{MazeNode(true, true, false, false), MazeNode(false, false, false, false), MazeNode(false, true, false, false), MazeNode(false, false, false, true), MazeNode(false, true, false, false), MazeNode(false, false, false, false), MazeNode(false, true, false, false), MazeNode(false, true, false, false)},
			{MazeNode(true, true, false, false), MazeNode(false, true, false, false), MazeNode(false, false, false, true), MazeNode(false, true, false, false), MazeNode(false, true, false, false), MazeNode(false, true, false, false), MazeNode(false, true, false, true), MazeNode(false, true, false, false)},
			{MazeNode(true, true, false, false), MazeNode(false, false, false, true), MazeNode(false, true, false, false), MazeNode(false, true, false, false), MazeNode(false, true, false, false), MazeNode(false, false, false, true), MazeNode(false, false, false, true), MazeNode(false, true, false, true)},
			{MazeNode(true, false, false, false), MazeNode(false, true, false, false), MazeNode(false, true, false, false), MazeNode(false, true, false, false), MazeNode(false, false, false, true), MazeNode(false, true, false, false), MazeNode(false, false, false, false), MazeNode(false, true, false, false)},
			{MazeNode(true, true, false, false), MazeNode(false, true, false, true), MazeNode(false, true, false, false), MazeNode(false, false, false, true), MazeNode(false, true, false, false), MazeNode(false, false, false, true), MazeNode(false, true, false, true), MazeNode(false, true, false, false)},
			{MazeNode(true, false, false, true), MazeNode(false, false, false, true), MazeNode(false, false, false, true), MazeNode(false, true, false, true), MazeNode(false, false, false, true), MazeNode(false, false, false, false), MazeNode(false, false, false, true), MazeNode(false, true, false, true)},
		};



		// Load some icons into the GPU
		for (auto& entry : std::filesystem::directory_iterator(std::string(ROOT_PATH) + "/assets/")) {

			std::string name = "";

			int path_size = strlen(entry.path().string().c_str());

			for (int i = path_size - 1; i >= 0; i--) {
				if (entry.path().string().c_str()[i] == '/' || entry.path().string().c_str()[i] == '\\') {
					name = std::string(&entry.path().string().c_str()[i + 1]);
					break;
				}
			}

			texture_library[name] = Engine::Renderer::Get()->UploadImageToGPU(entry.path().string().c_str(), false);
		}

		// Base floor entity
		Engine::Ref<Engine::Entity> floor = scene->CreateEntity("Floor", nullptr);
		Engine::TransformComponent floor_location(glm::vec3(0.f, 0.f, 0.f));
		floor->AddComponent<Engine::TransformComponent>(floor_location);
		entities[floor->GetID()] = floor;

		// Base maze entity
		Engine::Ref<Engine::Entity> maze = scene->CreateEntity("Maze", nullptr);
		Engine::TransformComponent maze_location(glm::vec3(0.f, 0.f, 0.f));
		maze->AddComponent<Engine::TransformComponent>(maze_location);
		entities[maze->GetID()] = maze;

		// Floor sectors
		float floor_sector_width = 0.25f;
		float floor_sector_height = 0.1f;
		float wall_thickness = 0.01f;
		float wall_height = 0.2f;

		for (int i = 0; i < 8; i++) {
			bool is_white = (i % 2 == 0);

			for (int j = 0; j < 8; j++) {
				std::string entity_name = "Floor_Sector_" + is_white ? "White_" : "Black_";
				entity_name += std::to_string(i) + "_" + std::to_string(j);
				Engine::Ref<Engine::Entity> floor_sector = scene->CreateEntity(entity_name, floor.get());

				glm::vec3 floor_sector_translation = glm::vec3((j - 4.0f) * floor_sector_width, 0.f, (i - 4.0f) * floor_sector_width);
				Engine::TransformComponent new_transform(floor_sector_translation);
				new_transform.local_transform.scale = glm::vec3(floor_sector_width, floor_sector_height, floor_sector_width);
				Engine::Ref<Engine::Mesh> mesh = Engine::CreateRef<Cuboid>(1.f, 1.f, 1.f);

				Engine::Ref<Engine::Material> material;
				if (is_white) {
					material = Engine::CreateRef<Pearl>();
				}
				else {
					material = Engine::CreateRef<Obsidian>();
				}
				floor_sector->AddComponent<Engine::MeshComponent>(mesh);
				floor_sector->AddComponent<Engine::MaterialComponent>(material);
				floor_sector->AddComponent<Engine::TransformComponent>(new_transform);
				entities[floor_sector->GetID()] = floor_sector;

				MazeNode* current_node = &maze_layout[i][j];

				if (current_node->left_wall) {
					Engine::TransformComponent transform(glm::vec3((j - 4.0f) * floor_sector_width - floor_sector_width/2, floor_sector_height/2 + wall_height/2, (i - 4.0f) * floor_sector_width));
					transform.local_transform.scale.x *= wall_thickness;
					transform.local_transform.scale.y *= wall_height;
					transform.local_transform.scale.z *= floor_sector_width;

					CreateWall(transform, maze.get(), "Wall_Left_" + std::to_string(i) + "_" + std::to_string(j));
				}

				if (current_node->right_wall) {
					Engine::TransformComponent transform(glm::vec3((j - 4.0f) * floor_sector_width + floor_sector_width / 2, floor_sector_height / 2 + wall_height / 2, (i - 4.0f) * floor_sector_width));
					transform.local_transform.scale.x *= wall_thickness;
					transform.local_transform.scale.y *= wall_height;
					transform.local_transform.scale.z *= floor_sector_width;

					CreateWall(transform, maze.get(), "Wall_Right_" + std::to_string(i) + "_" + std::to_string(j));
				}

				if (current_node->up_wall) {
					Engine::TransformComponent transform(glm::vec3((j - 4.0f) * floor_sector_width, floor_sector_height / 2 + wall_height / 2, (i - 4.0f) * floor_sector_width - floor_sector_width / 2));
					transform.local_transform.scale.x *= floor_sector_width;
					transform.local_transform.scale.y *= wall_height;
					transform.local_transform.scale.z *= wall_thickness;

					CreateWall(transform, maze.get(), "Wall_Up_" + std::to_string(i) + "_" + std::to_string(j));
				}

				if (current_node->down_wall) {
					Engine::TransformComponent transform(glm::vec3((j - 4.0f) * floor_sector_width, floor_sector_height / 2 + wall_height / 2, (i - 4.0f) * floor_sector_width + floor_sector_width / 2));
					transform.local_transform.scale.x *= floor_sector_width;
					transform.local_transform.scale.y *= wall_height;
					transform.local_transform.scale.z *= wall_thickness;

					CreateWall(transform, maze.get(), "Wall_Down_" + std::to_string(i) + "_" + std::to_string(j));
				}
				is_white = !is_white;
			}
		}

		// Some random primitives

		std::vector<float> random_coordinates{};

		for (int i = 0; i < 8; i++) {
			random_coordinates.push_back((i - 2.0f)* floor_sector_width - floor_sector_width);
		}

		float object_height = 0.05f;
		Engine::Ref<Engine::Mesh> cube = Engine::CreateRef<Cuboid>(1.f, 1.f, 1.f);
		Engine::Ref<Engine::Mesh> sphere = Engine::CreateRef<Sphere>(1.f, 100, 100);
		Engine::Ref<Engine::Mesh> cylinder = Engine::CreateRef<Cylinder>(1.f, 1.f, 100);
		Engine::Ref<Engine::Mesh> cone = Engine::CreateRef<Cone>(1.f, 1.f, 100);
		Engine::Ref<Engine::Mesh> torus = Engine::CreateRef<Torus>(1.f, 0.5f, 100, 100);

		Engine::Ref<Engine::Material> cube_material = Engine::CreateRef<Silver>();
		Engine::Ref<Engine::Material> sphere_material = Engine::CreateRef<Silver>();
		Engine::Ref<Engine::Material> cylinder_material = Engine::CreateRef<Silver>();
		Engine::Ref<Engine::Material> cone_material = Engine::CreateRef<Silver>();
		Engine::Ref<Engine::Material> torus_material = Engine::CreateRef<Silver>();

		// one sphere, one cube, one cylinder, one cone, and one torus
		Engine::Ref<Engine::Entity> cube_entity = scene->CreateEntity("Cube", nullptr);
		Engine::Ref<Engine::Entity> sphere_entity = scene->CreateEntity("Sphere", nullptr);
		Engine::Ref<Engine::Entity> cylinder_entity = scene->CreateEntity("Cylinder", nullptr);
		Engine::Ref<Engine::Entity> cone_entity = scene->CreateEntity("Cone", nullptr);
		Engine::Ref<Engine::Entity> torus_entity = scene->CreateEntity("Torus", nullptr);

		Engine::TransformComponent cube_transform(glm::vec3(random_coordinates[Engine::Random::RandomInt(0, 7)], floor_sector_height / 2 + object_height/2, random_coordinates[Engine::Random::RandomInt(0, 7)]));
		cube_transform.local_transform.scale = glm::vec3(object_height, object_height, object_height);

		Engine::TransformComponent sphere_transform(glm::vec3(random_coordinates[Engine::Random::RandomInt(0, 7)], floor_sector_height / 2 + object_height / 2, random_coordinates[Engine::Random::RandomInt(0, 7)]));
		sphere_transform.local_transform.scale = glm::vec3(object_height, object_height, object_height);

		Engine::TransformComponent cylinder_transform(glm::vec3(random_coordinates[Engine::Random::RandomInt(0, 7)], floor_sector_height / 2 + object_height / 2, random_coordinates[Engine::Random::RandomInt(0, 7)]));
		cylinder_transform.local_transform.scale = glm::vec3(object_height, object_height, object_height);

		Engine::TransformComponent cone_transform(glm::vec3(random_coordinates[Engine::Random::RandomInt(0, 7)], floor_sector_height / 2 + object_height / 2, random_coordinates[Engine::Random::RandomInt(0, 7)]));
		cone_transform.local_transform.scale = glm::vec3(object_height, object_height, object_height);

		Engine::TransformComponent torus_transform(glm::vec3(random_coordinates[Engine::Random::RandomInt(0, 7)], floor_sector_height / 2 + object_height, random_coordinates[Engine::Random::RandomInt(0, 7)]));
		torus_transform.local_transform.scale = glm::vec3(object_height, object_height, object_height);


		cube_entity->AddComponent<Engine::MeshComponent>(cube);
		cube_entity->AddComponent<Engine::MaterialComponent>(cube_material);
		cube_entity->AddComponent<Engine::TransformComponent>(cube_transform);

		sphere_entity->AddComponent<Engine::MeshComponent>(sphere);
		sphere_entity->AddComponent<Engine::MaterialComponent>(sphere_material);
		sphere_entity->AddComponent<Engine::TransformComponent>(sphere_transform);

		cylinder_entity->AddComponent<Engine::MeshComponent>(cylinder);
		cylinder_entity->AddComponent<Engine::MaterialComponent>(cylinder_material);
		cylinder_entity->AddComponent<Engine::TransformComponent>(cylinder_transform);

		cone_entity->AddComponent<Engine::MeshComponent>(cone);
		cone_entity->AddComponent<Engine::MaterialComponent>(cone_material);
		cone_entity->AddComponent<Engine::TransformComponent>(cone_transform);

		torus_entity->AddComponent<Engine::MeshComponent>(torus);
		torus_entity->AddComponent<Engine::MaterialComponent>(torus_material);
		torus_entity->AddComponent<Engine::TransformComponent>(torus_transform);

		entities[cube_entity->GetID()] = cube_entity;
		entities[sphere_entity->GetID()] = sphere_entity;
		entities[cylinder_entity->GetID()] = cylinder_entity;
		entities[cone_entity->GetID()] = cone_entity;
		entities[torus_entity->GetID()] = torus_entity;

		CreateRobot();

		Engine::Ref<Engine::Entity> light_entity = scene->CreateEntity("Sun", nullptr);
		Engine::Ref<Engine::PointLight> light = Engine::CreateRef<Engine::PointLight>(); // defaults to white
		Engine::TransformComponent new_transform(glm::vec3(0.f, 1.f, 0.f));
		Engine::PointLightComponent new_light(light);

		light_entity->AddComponent<Engine::TransformComponent>(new_transform);
		light_entity->AddComponent<Engine::PointLightComponent>(new_light);

		entities[light_entity->GetID()] = light_entity;

	}

	void RandomObjectsLayer::CreateWall(Engine::TransformComponent new_transform, Engine::Entity* maze_entity, const std::string& name) {
		
		Engine::Ref<Engine::Entity> maze_wall = scene->CreateEntity(name, maze_entity);
		// Add other components (Mesh, Material) to the wall entity
		Engine::Ref<Engine::Mesh> mesh = Engine::CreateRef<Cuboid>(1.f, 1.f, 1.f);
		Engine::Ref<Engine::Material> material = Engine::CreateRef<Silver>();

		maze_wall->AddComponent<Engine::MeshComponent>(mesh);
		maze_wall->AddComponent<Engine::MaterialComponent>(material);
		maze_wall->AddComponent<Engine::TransformComponent>(new_transform);

		// Store the wall entity
		entities[maze_wall->GetID()] = maze_wall;
	}

	void RandomObjectsLayer::CreateRobot()
	{
		// Create robot entity
		Engine::Ref<Engine::Entity> robot_entity = scene->CreateEntity("Robot", nullptr);
		Engine::TransformComponent robot_location(glm::vec3(0.f, 1.5f, 0.f)); // Position the robot above the ground
		robot_entity->AddComponent<Engine::TransformComponent>(robot_location);
		entities[robot_entity->GetID()] = robot_entity;

		// Define materials
		Engine::Ref<Engine::Material> robot_material = Engine::CreateRef<Silver>();
		Engine::Ref<Engine::Material> star_material = Engine::CreateRef<Gold>();

		// Define robot dimensions
		float body_height = 0.4f;
		float body_width = 0.15f;
		float body_thickness = 0.1f;
		float limb_width = 0.1f;
		float limb_height = 0.8f;
		float head_height = 0.4f;
		float head_width = 0.5f;

		// Create and add the body
		Engine::Ref<Engine::Mesh> body_mesh = Engine::CreateRef<Cuboid>(1.f, 1.f, 1.f);
		Engine::TransformComponent body_transform(glm::vec3(0.f, 0.25, 0.f));
		body_transform.local_transform.scale = glm::vec3(body_width, body_height/2, body_thickness);
		robot_entity->AddComponent<Engine::MeshComponent>(body_mesh);
		robot_entity->AddComponent<Engine::MaterialComponent>(robot_material);
		robot_entity->AddComponent<Engine::TransformComponent>(body_transform);

		// Create and add the head
		Engine::Ref<Engine::Entity> head_entity = scene->CreateEntity("Head", robot_entity.get());
		Engine::Ref<Engine::Mesh> head_mesh = Engine::CreateRef<Cuboid>(1.f, 1.f, 1.f);
		Engine::TransformComponent head_transform(glm::vec3(0.f, 0.5f + head_height/2, 0.f)); // Position on top of the body
		head_transform.local_transform.scale = glm::vec3(head_width, head_height, 0.5f);
		head_entity->AddComponent<Engine::MeshComponent>(head_mesh);
		head_entity->AddComponent<Engine::MaterialComponent>(robot_material);
		head_entity->AddComponent<Engine::TransformComponent>(head_transform);
		entities[head_entity->GetID()] = head_entity;
		m_HeadEntity = head_entity;

		// Create and add the left arm
		Engine::Ref<Engine::Entity> left_arm_entity = scene->CreateEntity("Left_Arm", robot_entity.get());
		Engine::Ref<Engine::Mesh> left_arm_mesh = Engine::CreateRef<Cuboid>(1.f, 1.f, 1.f);
		Engine::TransformComponent left_arm_transform(glm::vec3(-1.f/2, 0.f, 0.f));
		left_arm_transform.local_transform.scale = glm::vec3(limb_width, limb_height, 0.3f);
		left_arm_entity->AddComponent<Engine::MeshComponent>(left_arm_mesh);
		left_arm_entity->AddComponent<Engine::MaterialComponent>(robot_material);
		left_arm_entity->AddComponent<Engine::TransformComponent>(left_arm_transform);
		entities[left_arm_entity->GetID()] = left_arm_entity;

		// Create and add the right arm
		Engine::Ref<Engine::Entity> right_arm_entity = scene->CreateEntity("Right_Arm", robot_entity.get());
		Engine::Ref<Engine::Mesh> right_arm_mesh = Engine::CreateRef<Cuboid>(1.f, 1.f, 1.f);
		Engine::TransformComponent right_arm_transform(glm::vec3(1.f / 2, 0.f, 0.f));
		right_arm_transform.local_transform.scale = glm::vec3(limb_width, limb_height, 0.3f);
		right_arm_entity->AddComponent<Engine::MeshComponent>(right_arm_mesh);
		right_arm_entity->AddComponent<Engine::MaterialComponent>(robot_material);
		right_arm_entity->AddComponent<Engine::TransformComponent>(right_arm_transform);
		entities[right_arm_entity->GetID()] = right_arm_entity;

		Engine::Ref<Engine::Entity> left_leg_entity = scene->CreateEntity("Left_Leg", robot_entity.get());
		Engine::Ref<Engine::Mesh> left_leg_mesh = Engine::CreateRef<Cuboid>(1.f, 1.f, 1.f);
		Engine::TransformComponent left_leg_transform(glm::vec3(-0.3, -0.5, 0.f)); // Position at the bottom of the body
		left_leg_transform.local_transform.scale = glm::vec3(limb_width, 1.f, 0.3f);
		left_leg_entity->AddComponent<Engine::MeshComponent>(left_leg_mesh);
		left_leg_entity->AddComponent<Engine::MaterialComponent>(robot_material);
		left_leg_entity->AddComponent<Engine::TransformComponent>(left_leg_transform);
		entities[left_leg_entity->GetID()] = left_leg_entity;

		// Create and add the right leg
		Engine::Ref<Engine::Entity> right_leg_entity = scene->CreateEntity("Right_Leg", robot_entity.get());
		Engine::Ref<Engine::Mesh> right_leg_mesh = Engine::CreateRef<Cuboid>(1.f, 1.f, 1.f);
		Engine::TransformComponent right_leg_transform(glm::vec3(0.3, -0.5, 0.f)); // Position at the bottom of the body
		right_leg_transform.local_transform.scale = glm::vec3(limb_width, 1.f, 0.3f);
		right_leg_entity->AddComponent<Engine::MeshComponent>(right_leg_mesh);
		right_leg_entity->AddComponent<Engine::MaterialComponent>(robot_material);
		right_leg_entity->AddComponent<Engine::TransformComponent>(right_leg_transform);
		entities[right_leg_entity->GetID()] = right_leg_entity;

		// Create and add the star on top of the head
		Engine::Ref<Engine::Entity> star_entity = scene->CreateEntity("Star", head_entity.get());
		Engine::Ref<Engine::Mesh> star_mesh = Engine::CreateRef<ChristmasStar>(0.3f, 0.5f, 0.2f, 7);
		Engine::TransformComponent star_transform(glm::vec3(0.f, 1.f, 0.f)); // Adjust Y position to place on top of head
		star_transform.local_transform.scale = glm::vec3(1.f, 1.0f, 0.4f);
		star_entity->AddComponent<Engine::MeshComponent>(star_mesh);
		star_entity->AddComponent<Engine::MaterialComponent>(star_material);
		star_entity->AddComponent<Engine::TransformComponent>(star_transform);
		entities[star_entity->GetID()] = star_entity;
		m_StarEntity = star_entity;

	}

	void RandomObjectsLayer::OnDetach()
	{

	}
	void RandomObjectsLayer::OnUpdate(float delta_time)
	{
		animationTime += delta_time;

		float sineWave = sin(animationTime);

		float rotationAngle = 90.0f * sineWave;
		float translationDistance = 0.2f * sineWave;

		auto& head_transform = m_HeadEntity->GetComponent<Engine::TransformComponent>()->local_transform;
		head_transform.rotation.y = glm::radians(rotationAngle);
		head_transform.translation.x = translationDistance;

		float scale_factor = (sineWave + 1) / 2;
		float min_scale = 0.5f;
		float max_scale = 1.0f;
		float current_scale = min_scale + (max_scale - min_scale) * scale_factor;

		auto& star_transform = m_StarEntity->GetComponent<Engine::TransformComponent>()->local_transform;
		star_transform.scale = glm::vec3(current_scale, current_scale, current_scale);
	}
	void RandomObjectsLayer::OnEvent(Engine::Event& event)
	{

	}

	void RandomObjectsLayer::DisplaySceneNode(Engine::SceneNode* node) {
		if (node == nullptr || entities[node->GetID()] == nullptr) {
			return;
		}

		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
		node_flags |= ImGuiTreeNodeFlags_DefaultOpen;

		bool nodeIsOpen = ImGui::TreeNodeEx((void*)(intptr_t)node->GetID(), node_flags, "%s", "");
		ImGui::SameLine();
		ImGui::Image((void*)(intptr_t)texture_library["entity.png"].texture, ImVec2(16, 16));
		ImGui::SameLine();
		ImGui::Text(entities[node->GetID()]->GetName().c_str());

		if (nodeIsOpen) {
			
			DisplayComponents(node->GetID());
			if (node->GetChildren()->size() > 0) {
				std::string tree_title = "(children) [" + std::to_string(node->GetChildren()->size()) + "]";
				if (ImGui::TreeNodeEx((void*)(intptr_t)child_tree_node_id, ImGuiTreeNodeFlags_OpenOnArrow, "%s", tree_title.c_str())) {
					for (auto& child : *node->GetChildren()) {
						ImGui::Dummy(ImVec2(0.0f, 5.0f));
						DisplaySceneNode(child.get());
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}

	void RandomObjectsLayer::DisplayComponents(Engine::UUID entity_id)
	{
		selected_option[entity_id][Engine::Transform] = currently_selected.first == entity_id && currently_selected.second == Engine::Transform ? true : false;
		selected_option[entity_id][Engine::MeshComp] = currently_selected.first == entity_id && currently_selected.second == Engine::MeshComp ? true : false;
		selected_option[entity_id][Engine::MaterialComp] = currently_selected.first == entity_id && currently_selected.second == Engine::MaterialComp ? true : false;
		selected_option[entity_id][Engine::CameraComp] = currently_selected.first == entity_id && currently_selected.second == Engine::CameraComp ? true : false;
		selected_option[entity_id][Engine::PointLightComp] = currently_selected.first == entity_id && currently_selected.second == Engine::PointLightComp ? true : false;
		ImGui::Indent(5.f);
		if (entities[entity_id]->HasComponent<Engine::TransformComponent>()) {
			ImGui::Image((void*)(intptr_t)texture_library["axis.png"].texture, ImVec2(16, 16));
			ImGui::SameLine();
			if (ImGui::Selectable("Transform", &selected_option[entity_id][Engine::Transform])) {
				currently_selected = { entity_id, Engine::Transform };
				entity_selected = entity_id;
				component_selected = entities[entity_id]->GetComponent<Engine::TransformComponent>();
			}
		}
		if (entities[entity_id]->HasComponent<Engine::MeshComponent>()) {
			ImGui::Image((void*)(intptr_t)texture_library["mesh.png"].texture, ImVec2(16, 16), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::SameLine();
			if (ImGui::Selectable("Mesh", &selected_option[entity_id][Engine::MeshComp])) {
				currently_selected = { entity_id, Engine::MeshComp };
				entity_selected = entity_id;
				component_selected = entities[entity_id]->GetComponent<Engine::MeshComponent>();
			}
			
		}

		if (entities[entity_id]->HasComponent<Engine::MaterialComponent>()) {
			ImGui::Image((void*)(intptr_t)texture_library["material.png"].texture, ImVec2(16, 16), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::SameLine();
			if (ImGui::Selectable("Material", &selected_option[entity_id][Engine::MaterialComp])) {
				currently_selected = { entity_id, Engine::MaterialComp };
				entity_selected = entity_id;
				component_selected = entities[entity_id]->GetComponent<Engine::MaterialComponent>();
			}

		}
		if (entities[entity_id]->HasComponent<Engine::CameraComponent>()) {
			ImGui::Image((void*)(intptr_t)texture_library["camera.png"].texture, ImVec2(16, 16), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::SameLine();
			if (ImGui::Selectable("Camera", &selected_option[entity_id][Engine::CameraComp])) {
				currently_selected = { entity_id, Engine::CameraComp };
				entity_selected = entity_id;
				component_selected = entities[entity_id]->GetComponent<Engine::CameraComponent>();
			}
			
		}
		if (entities[entity_id]->HasComponent<Engine::PointLightComponent>()) {
			ImGui::Image((void*)(intptr_t)texture_library["light.png"].texture, ImVec2(16, 16), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::SameLine();
			if (ImGui::Selectable("Point Light", &selected_option[entity_id][Engine::PointLightComp])) {
				currently_selected = { entity_id, Engine::PointLightComp };
				entity_selected = entity_id;
				component_selected = entities[entity_id]->GetComponent<Engine::PointLightComponent>();
			}

		}
		ImGui::Unindent(5.f);
	}

	void RandomObjectsLayer::DisplayDetails()
	{
		if (component_selected->Type() == Engine::Transform) {
			ImGui::Text("Translation:");
			float translation[3] = {entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.translation.x, entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.translation.y, entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.translation.z};
			ImGui::DragFloat3("##translate", translation, 0.001f);
			entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.translation.x = translation[0];
			entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.translation.y = translation[1];
			entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.translation.z = translation[2];

			ImGui::Text("Rotation:");
			float rotation[3] = { entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.rotation.x, entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.rotation.y, entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.rotation.z };
			ImGui::DragFloat3("##rotate", rotation, 0.001f);
			entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.rotation.x = rotation[0];
			entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.rotation.y = rotation[1];
			entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.rotation.z = rotation[2];

			ImGui::Text("Scale:");
			float scale[3] = { entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.scale.x, entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.scale.y, entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.scale.z };
			ImGui::DragFloat3("##scale", scale, 0.001f);
			entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.scale.x = scale[0];
			entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.scale.y = scale[1];
			entities[entity_selected]->GetComponent<Engine::TransformComponent>()->local_transform.scale.z = scale[2];
		}
		else if (component_selected->Type() == Engine::CameraComp) {
			if (ImGui::Button("Set active")) {
				scene->SetCurrentCamera(entity_selected);
			}
			ImGui::Text("Pitch and Yaw:");
			float camera_rotation[2] = { entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->GetPitch(), entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->GetYaw() };
			ImGui::DragFloat2("##pitch_yaw", camera_rotation, 0.01f);
			entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->SetPitch(camera_rotation[0]);
			entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->SetYaw(camera_rotation[1]);

			bool is_perspective = entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->GetType() == Engine::CameraType::PERSPECTIVE;
			ImGui::Text("Perspective view:");
			ImGui::SameLine();
			ImGui::Checkbox("##is_perspective", &is_perspective);
			if (is_perspective) {
				entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->ChangeCameraType(Engine::CameraType::PERSPECTIVE);
			}
			else {
				entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->ChangeCameraType(Engine::CameraType::ORTHOGRAPHIC);
			}
			if (is_perspective) {
				ImGui::SameLine();
				ImGui::Text("Lock camera rotation:");
				ImGui::SameLine();
				ImGui::Checkbox("##camera_lock", &entities[entity_selected]->GetComponent<Engine::CameraComponent>()->lock_camera);

				float distance = entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->GetFar();
				ImGui::Text("View Distance:");
				ImGui::SameLine();
				ImGui::DragFloat("##view_distance", &distance, 1.f);
				entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->SetFar(distance);

				int fovy = (int)entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->GetFOVY();
				ImGui::Text("Field of view:");
				ImGui::SameLine();
				ImGui::DragInt("##fovy", &fovy, 1);
				entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->SetFovy((float)fovy);

				float zoom = entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->GetZoomDistance();
				ImGui::Text("Zoom:");
				ImGui::SameLine();
				ImGui::DragFloat("##zoom", &zoom, 0.1f);
				entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->SetZoomDistance(zoom);
			}
			else {
				float orthographic_size = entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->GetOrthographicSize();
				ImGui::Text("Orthographic Size:");
				ImGui::SameLine();
				ImGui::DragFloat("##ortho_size", &orthographic_size, 0.1f);
				entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->SetOrthographicSize(orthographic_size);
			}
		}
		else if (component_selected->Type() == Engine::PointLightComp) {
			float color[3] = { entities[entity_selected]->GetComponent<Engine::PointLightComponent>()->light->GetColor().r,  entities[entity_selected]->GetComponent<Engine::PointLightComponent>()->light->GetColor().g, entities[entity_selected]->GetComponent<Engine::PointLightComponent>()->light->GetColor().b };
			ImGui::Text("Light color:");
			ImGui::ColorEdit3("##light_color", color);
			entities[entity_selected]->GetComponent<Engine::PointLightComponent>()->light->SetColor(glm::vec3(color[0], color[1], color[2]));

			float intensity = entities[entity_selected]->GetComponent<Engine::PointLightComponent>()->light->GetIntensity();
			ImGui::Text("Intensity:");
			ImGui::DragFloat("##light_intensity", &intensity, 0.1f);
			entities[entity_selected]->GetComponent<Engine::PointLightComponent>()->light->SetIntensity(intensity);

			float attenuations[3] = { entities[entity_selected]->GetComponent<Engine::PointLightComponent>()->light->GetConstantAttenuation(),  entities[entity_selected]->GetComponent<Engine::PointLightComponent>()->light->GetLinearAttenuation(), entities[entity_selected]->GetComponent<Engine::PointLightComponent>()->light->GetQuadraticAttenuation()};
			ImGui::Text("Attenuations (constant, linear, quadratic):");
			ImGui::DragFloat3("##light_attenuation", attenuations, 0.1f);
			entities[entity_selected]->GetComponent<Engine::PointLightComponent>()->light->SetConstantAttenuation(attenuations[0]);
			entities[entity_selected]->GetComponent<Engine::PointLightComponent>()->light->SetLinearAttenuation(attenuations[1]);
			entities[entity_selected]->GetComponent<Engine::PointLightComponent>()->light->SetQuadraticAttenuation(attenuations[2]);


		}
		else {
			ImGui::Text("Nothing to show for this component!");
		}
	}

	void RandomObjectsLayer::OnImGuiRender()
	{

		float windowWidth = ImGui::GetContentRegionAvail().x;
		float columnWidth = windowWidth * 0.5f;

		ImGui::BeginChild("Stats", ImVec2(windowWidth, 50), true);
		ImGui::Text("Stats");
		std::string fps_text = "FPS: " + std::to_string(Engine::Renderer::Get()->GetStats()->frames_per_second);
		ImGui::Text(fps_text.c_str());
		ImGui::EndChild();

		ImGui::BeginChild("Scene graph", ImVec2(columnWidth, 0), true);

		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
		node_flags |= ImGuiTreeNodeFlags_DefaultOpen;

		std::string root_node_name = "("+scene->GetName() + ")";
		if (ImGui::TreeNodeEx((void*)(intptr_t)scene->GetId(), node_flags, "%s", root_node_name.c_str())) {
			for (const auto& child : *scene->GetRootNode()->GetChildren()) {
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				DisplaySceneNode(child.get());
			}
			ImGui::TreePop();
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Details", ImVec2(0, 0), true); 

		if (component_selected && entities.find(entity_selected) != entities.end()) {
			DisplayDetails();
		}
		else {
			ImGui::Text("No component selected");
		}
		ImGui::EndChild();

	}
}