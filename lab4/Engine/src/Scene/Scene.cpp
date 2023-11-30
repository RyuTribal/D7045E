#include "pch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include <glm/gtx/matrix_decompose.hpp>
#include "Renderer/Renderer.h"

namespace Engine {

	std::pair<Ref<Entity>, Ref<Scene>> Scene::CreateScene(std::string name)
	{
		Ref<Scene> scene = CreateRef<Scene>(name);
		Ref<Entity> default_camera_entity = scene->CreateEntity("Default scene camera", nullptr);
		scene->SetCurrentCamera(default_camera_entity->GetID());
		Ref<Camera> camera = CreateRef<Camera>(CameraType::PERSPECTIVE);
		default_camera_entity->AddComponent<CameraComponent>(CameraComponent(camera));
		glm::vec3 default_transform = glm::vec3(0.f, 0.f, 0.f);
		default_camera_entity->AddComponent<LocalTransformComponent>(LocalTransformComponent(default_transform));

		return {default_camera_entity, scene};
	}


	Scene::Scene(std::string name)
		: m_Name(name)
	{
	}

	Scene::~Scene() {
		for (auto [key, entity] : entities) {
			m_Registry.RemoveAllFromEntity(key);
		}
	}
	Ref<Entity> Scene::CreateEntity(std::string name, Entity* parent) {
		
		UUID new_id = UUID();
		m_RootSceneNode.AddChild(parent == nullptr ? m_ID : parent->GetID(), new_id);

		m_Registry.Add<ParentIDComponent>(new_id, ParentIDComponent(parent == nullptr ? m_ID : parent->GetID()));
		m_Registry.Add<WorldTransformComponent>(new_id, WorldTransformComponent());

		Ref<Entity> new_entity = CreateRef<Entity>(new_id, name, this);

		entities[new_id] = new_entity->GetHandle();

		return new_entity;
	}

	void Scene::DestroyEntity(UUID id)
	{
		m_RootSceneNode.RemoveChild(id, &m_RootSceneNode);
		m_Registry.RemoveAllFromEntity(id);
		entities.erase(id);
	}

	Camera* Scene::GetCurrentCamera(){
		return m_Registry.Get<CameraComponent>(m_CurrentCamera)->camera.get();
	}
	void Scene::UpdateScene(int delta_time)
	{
		UpdateTransforms();
		DrawSystem();
	}

	void Scene::UpdateWorldTransform(SceneNode* node, glm::mat4& parentWorldTransform)
	{
		if (!node) return;

		auto localTransform = m_Registry.Get<LocalTransformComponent>(node->GetID());
		glm::mat4 worldTransform = localTransform ? parentWorldTransform * localTransform->mat4() : parentWorldTransform;

		auto worldTransformComponent = m_Registry.Get<WorldTransformComponent>(node->GetID());
		if (worldTransformComponent) {
			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;

			glm::decompose(worldTransform, scale, rotation, translation, skew, perspective);

			worldTransformComponent->translation = translation;
			worldTransformComponent->rotation = glm::eulerAngles(rotation);
			worldTransformComponent->scale = scale;
		}

		for (auto& child : *node->GetChildren()) {
			UpdateWorldTransform(child.get(), worldTransform);
		}
	}

	void Scene::UpdateCamera()
	{
		auto camera = m_Registry.Get<CameraComponent>(m_CurrentCamera)->camera;

		glm::quat camera_orientation = camera->GetOrientation();
		glm::vec3 camera_position = camera->GetPosition();


	}

	void Scene::UpdateTransforms()
	{
		glm::mat4 identityMatrix = glm::mat4(1.0f);

		for (auto& child : *m_RootSceneNode.GetChildren()) {
			UpdateWorldTransform(child.get(), identityMatrix);
		}
	}

	void Scene::DrawSystem()
	{
		if (m_Registry.GetComponentRegistry<ObjectComponent>() != nullptr) {
			for (const auto& [id, value] : *m_Registry.GetComponentRegistry<ObjectComponent>()) {
				if (m_Registry.Get<WorldTransformComponent>(id) != nullptr) {
					value.node->Draw(m_Registry.Get<WorldTransformComponent>(id)->mat4());
				}
				else {
					value.node->Draw(glm::mat4(0.f)); // just a default one, adds no transform
				}
			}
		}
	}


}