#pragma once
#include "Mesh.h"
#include "Material.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Materials/DebugLines.h>
#include <Materials/DebugPoints.h>

namespace Engine {

	struct TransformComponent {
		glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 mat4() {
			glm::mat4 Rotation = glm::toMat4(glm::quat(rotation));

			return glm::translate(glm::mat4(1.0f), translation)
				* Rotation
				* glm::scale(glm::mat4(1.0f), scale);
		}
	};

	class GraphicsNode {
	public:
		GraphicsNode(Ref<Mesh> mesh, Ref<Material> material, glm::vec3 translation, glm::vec3 scale, glm::vec3 rotation) 
			: m_Mesh(mesh), m_Material(material), m_Transform()
		{
			m_Transform.translation = translation;
			m_Transform.scale = scale;
			m_Transform.rotation = rotation;
		}
		void Bind();
		void Draw();

		void SetShowFaces(bool show_faces) { b_ShowFaces = show_faces; }
		void SetShowLines(bool show_lines) { b_ShowLines = show_lines; }
		void SetShowVertices(bool show_vertices) { b_ShowVertices = show_vertices; }

		void Move(glm::vec3 velocity) { m_Transform.translation += velocity; }

		TransformComponent& GetTransform() { return m_Transform; }

	private:
		Ref<Mesh> m_Mesh;
		Ref<Material> m_Material;

		TransformComponent m_Transform;

		bool b_ShowFaces = true;
		bool b_ShowLines = false;
		bool b_ShowVertices = false;

		Ref<Material> m_DebugEdgeMaterial = CreateRef<DebugLines>();
		Ref<Material> m_DebugPointMaterial = CreateRef<DebugPoints>();
	};
}