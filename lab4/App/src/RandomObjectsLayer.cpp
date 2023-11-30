#include "RandomObjectsLayer.h"
#include <imgui/imgui.h>
#include <filesystem>

namespace Lab4 {
	void RandomObjectsLayer::OnAttach()
	{
		CORE_WARN("Random objects layer attached");

		Engine::Ref<Engine::Mesh> mesh = Engine::CreateRef<Engine::Cuboid>(0.5f, 0.5f, 0.5f);

		Engine::Ref<Engine::Material> material = Engine::CreateRef<Engine::Monochrome>(255, 0, 0);

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

		Engine::Ref<Engine::Entity> test_ent;

		for (int i = 0; i < random_object_count; i++) {
			float random_x = Engine::Random::RandomFloat(-0.7f, 0.7f);
			float random_y = Engine::Random::RandomFloat(-0.7f, 0.7f);
			float random_z = Engine::Random::RandomFloat(-3.f, -1.f); // so that it doesnt clip the camera since camera is at origin

			float random_scale = Engine::Random::RandomFloat(0.f, 0.5f);

			Engine::Ref<Engine::Entity> new_entity = scene->CreateEntity("Cube_" + std::to_string(i), i == random_object_count -1 ? test_ent.get() : nullptr);

			Engine::ObjectComponent new_mesh(Engine::CreateRef<Engine::GraphicsNode>(mesh, material));

			if (i == random_object_count - 2) {
				test_ent = new_entity;
			}

			glm::vec3 random_translation = glm::vec3(random_x, random_y, random_z);
			Engine::LocalTransformComponent new_transform(random_translation);
			new_transform.scale = glm::vec3(random_scale, random_scale, random_scale);
			
			new_entity->AddComponent<Engine::ObjectComponent>(new_mesh);
			new_entity->AddComponent<Engine::LocalTransformComponent>(new_transform);
			entities[new_entity->GetID()] = new_entity;
		}
	}
	void RandomObjectsLayer::OnDetach()
	{

	}
	void RandomObjectsLayer::OnUpdate(float delta_time)
	{
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
		selected_option[entity_id][Engine::LocalTransform] = currently_selected.first == entity_id && currently_selected.second == Engine::LocalTransform ? true : false;
		selected_option[entity_id][Engine::Object] = currently_selected.first == entity_id && currently_selected.second == Engine::Object ? true : false;
		selected_option[entity_id][Engine::CameraComp] = currently_selected.first == entity_id && currently_selected.second == Engine::CameraComp ? true : false;
		ImGui::Indent(5.f);
		if (entities[entity_id]->HasComponent<Engine::LocalTransformComponent>()) {
			ImGui::Image((void*)(intptr_t)texture_library["axis.png"].texture, ImVec2(16, 16));
			ImGui::SameLine();
			if (ImGui::Selectable("Transform", &selected_option[entity_id][Engine::LocalTransform])) {
				currently_selected = { entity_id, Engine::LocalTransform };
				entity_selected = entity_id;
				component_selected = entities[entity_id]->GetComponent<Engine::LocalTransformComponent>();
			}
		}
		if (entities[entity_id]->HasComponent<Engine::ObjectComponent>()) {
			ImGui::Image((void*)(intptr_t)texture_library["graphics.png"].texture, ImVec2(16, 16), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::SameLine();
			if (ImGui::Selectable("Graphics node", &selected_option[entity_id][Engine::Object])) {
				currently_selected = { entity_id, Engine::Object };
				entity_selected = entity_id;
				component_selected = entities[entity_id]->GetComponent<Engine::ObjectComponent>();
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
		ImGui::Unindent(5.f);
	}

	void RandomObjectsLayer::DisplayDetails()
	{
		if (component_selected->Type() == Engine::LocalTransform) {
			ImGui::Text("Translation:");
			float translation[3] = {entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->translation.x, entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->translation.y, entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->translation.z};
			ImGui::DragFloat3("##translate", translation, 0.01f);
			entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->translation.x = translation[0];
			entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->translation.y = translation[1];
			entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->translation.z = translation[2];

			ImGui::Text("Rotation:");
			float rotation[3] = { entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->rotation.x, entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->rotation.y, entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->rotation.z };
			ImGui::DragFloat3("##rotate", rotation, 0.01f);
			entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->rotation.x = rotation[0];
			entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->rotation.y = rotation[1];
			entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->rotation.z = rotation[2];

			ImGui::Text("Scale:");
			float scale[3] = { entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->scale.x, entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->scale.y, entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->scale.z };
			ImGui::DragFloat3("##scale", scale, 0.01f);
			entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->scale.x = scale[0];
			entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->scale.y = scale[1];
			entities[entity_selected]->GetComponent<Engine::LocalTransformComponent>()->scale.z = scale[2];
		}
		else if (component_selected->Type() == Engine::CameraComp) {

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
				ImGui::Text("Pitch and Yaw:");
				float camera_rotation[2] = { entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->GetPitch(), entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->GetYaw() };
				ImGui::DragFloat2("##pitch_yaw", camera_rotation, 0.01f);

				entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->SetPitch(camera_rotation[0]);
				entities[entity_selected]->GetComponent<Engine::CameraComponent>()->camera->SetYaw(camera_rotation[1]);

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
		else {
			ImGui::Text("Nothing to show for this component!");
		}
	}

	void RandomObjectsLayer::OnImGuiRender()
	{

		float windowWidth = ImGui::GetContentRegionAvail().x;
		float columnWidth = windowWidth * 0.5f;

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

		for (auto [key, entity] : entities) {
			if (entity->HasComponent<Engine::ObjectComponent>()) {
				entities[key]->GetComponent<Engine::ObjectComponent>()->node->SetShowLines(false);
			}
		}

		if (component_selected && entities.find(entity_selected) != entities.end()) {
			if (entities[entity_selected]->HasComponent<Engine::ObjectComponent>()) {
				entities[entity_selected]->GetComponent<Engine::ObjectComponent>()->node->SetShowLines(true);
			}
			DisplayDetails();
		}
		else {
			ImGui::Text("No component selected");
		}
		ImGui::EndChild();

	}
}