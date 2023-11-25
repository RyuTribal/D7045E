#include "pch.h"
#include "Mesh.h"

namespace Engine {
	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<int> indices)
	{
		SetData(vertices, indices);
	}
	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &m_VertexArrayObject);
		glDeleteBuffers(1, &m_VertexBufferObject);
		glDeleteBuffers(1, &m_IndexBufferObject);
	}
	void Mesh::SetData(std::vector<Vertex>& vertices, std::vector<int> indices)
	{
		for (Vertex& vertex : vertices) {
			Ref<Vertex> pointer_vertex = CreateRef<Vertex>(vertex.x, vertex.y, vertex.z);
			pointer_vertex->SetColor(vertex.r, vertex.g, vertex.b);
			pointer_vertex->SetOpacity(vertex.a);
			m_Vertices.push_back(pointer_vertex);
		}


		for (int index : indices) {
			m_Indices.push_back(index);
		}

		glCreateVertexArrays(1, &m_VertexArrayObject);
		glCreateBuffers(1, &m_VertexBufferObject);
		glCreateBuffers(1, &m_IndexBufferObject);
	}
}