#pragma once
#include "Mesh.h"
#include "Material.h"
#include <glm/gtx/quaternion.hpp>

namespace Engine {

	struct TransformComponent {
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::vec3 rotation{};

		glm::mat4 mat4() {
			glm::mat4 rotation = glm::toMat4(glm::quat(rotation));

			return glm::translate(glm::mat4(1.0f), translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), scale);
		}
	};

	class GraphicsNode {
	public:
		GraphicsNode(Ref<Mesh> mesh, Ref<Material> material, glm::vec3 translation, glm::vec3 scale) 
			: m_Mesh(mesh), m_Material(material), m_Transform()
		{
			m_Transform.translation = translation;
			m_Transform.scale = scale;
		}
		void Draw();

		void SetShowFaces(bool show_faces) { b_ShowFaces = show_faces; }
		void SetShowLines(bool show_lines) { b_ShowLines = show_lines; }
		void SetShowVertices(bool show_vertices) { b_ShowVertices = show_vertices; }

	private:
		Ref<Mesh> m_Mesh;
		Ref<Material> m_Material;

		TransformComponent m_Transform;

		bool b_ShowFaces = true;
		bool b_ShowLines = false;
		bool b_ShowVertices = false;
	};
}