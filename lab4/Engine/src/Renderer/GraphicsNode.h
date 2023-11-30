#pragma once
#include "Mesh.h"
#include "Material.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Materials/DebugLines.h>
#include <Materials/DebugPoints.h>

namespace Engine {

	class GraphicsNode {
	public:
		GraphicsNode(Ref<Mesh> mesh, Ref<Material> material) 
			: m_Mesh(mesh), m_Material(material)
		{
		}
		void Bind();
		void Draw(glm::mat4 transform);

		void SetShowFaces(bool show_faces) { b_ShowFaces = show_faces; }
		void SetShowLines(bool show_lines) { b_ShowLines = show_lines; }
		void SetShowVertices(bool show_vertices) { b_ShowVertices = show_vertices; }

		void SetTexture(GLuint texture) { m_Texture = texture; }
		void RemoveTexture() { delete &m_Texture; }

	private:
		Ref<Mesh> m_Mesh;
		Ref<Material> m_Material;

		GLuint m_Texture;

		bool b_ShowFaces = true;
		bool b_ShowLines = false;
		bool b_ShowVertices = false;

		Ref<Material> m_DebugEdgeMaterial = CreateRef<DebugLines>();
		Ref<Material> m_DebugPointMaterial = CreateRef<DebugPoints>();
	};
}