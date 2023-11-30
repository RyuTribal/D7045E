#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Renderer/GraphicsNode.h"
#include "Renderer/Camera.h"

namespace Engine {

	enum ComponentType {
		ID,
		ParentID,
		LocalTransform,
		WorldTransform,
		Object,
		CameraComp
	};

	struct Component {
		virtual const ComponentType Type() const = 0;
	};

	struct IDComponent : public Component {
		UUID id;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(UUID new_id) : id(new_id){}

		const ComponentType Type() const override {
			return ComponentType::ID;
		}
	};

	struct ParentIDComponent : public Component {
		UUID id;

		ParentIDComponent() = default;
		ParentIDComponent(const ParentIDComponent&) = default;
		ParentIDComponent(UUID new_id) : id(new_id) {}

		const ComponentType Type() const override {
			return ComponentType::ParentID;
		}
	};

	struct LocalTransformComponent : public Component {
		glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		LocalTransformComponent() = default;
		LocalTransformComponent(const LocalTransformComponent&) = default;
		LocalTransformComponent(glm::vec3& new_translation) : translation(new_translation) {}

		glm::mat4 mat4() {
			glm::mat4 Rotation = glm::toMat4(glm::quat(rotation));

			return glm::translate(glm::mat4(1.0f), translation)
				* Rotation
				* glm::scale(glm::mat4(1.0f), scale);
		}

		const ComponentType Type() const override {
			return ComponentType::LocalTransform;
		}
	};

	struct WorldTransformComponent : public Component {
		glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		WorldTransformComponent() = default;
		WorldTransformComponent(const WorldTransformComponent&) = default;
		WorldTransformComponent(glm::vec3& new_translation) : translation(new_translation) {}

		glm::mat4 mat4() {
			glm::mat4 Rotation = glm::toMat4(glm::quat(rotation));

			return glm::translate(glm::mat4(1.0f), translation)
				* Rotation
				* glm::scale(glm::mat4(1.0f), scale);
		}

		const ComponentType Type() const override {
			return ComponentType::LocalTransform;
		}
	};

	struct ObjectComponent : public Component {
		Ref<GraphicsNode> node;

		ObjectComponent() = default;
		ObjectComponent(const ObjectComponent&) = default;
		ObjectComponent(Ref<GraphicsNode> new_node) : node(new_node) {}

		const ComponentType Type() const override {
			return ComponentType::Object;
		}
	};


	struct CameraComponent : public Component {
		Ref<Camera> camera;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(Ref<Camera> new_camera) : camera(new_camera) {}

		const ComponentType Type() const override {
			return ComponentType::CameraComp;
		}
	};

}