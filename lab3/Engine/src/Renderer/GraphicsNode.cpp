#include "pch.h"
#include "GraphicsNode.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine{
	void GraphicsNode::Draw()
	{
		glBindVertexArray(m_Mesh->GetVertexArrayObject());
		glBindBuffer(GL_ARRAY_BUFFER, m_Mesh->GetVertexBuffer());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Mesh->GetIndexBuffer());

		std::vector<float> buffer = {};

		for (auto vertex : m_Mesh->GetVertices())
		{
			vertex->TransformData(buffer);
		}

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Mesh->GetIndices().size() * sizeof(int), m_Mesh->GetIndices().data(), GL_STATIC_DRAW);

		GLsizei stride = 7 * sizeof(float); // 3 for position + 4 for color, in bytes
		glBindBuffer(GL_ARRAY_BUFFER, m_Mesh->GetVertexBuffer());
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		if (b_ShowFaces) {
			m_Material->ApplyMaterial();
			int camera_matrix_location = glGetUniformLocation(m_Material->GetProgram()->GetProgram(), "u_CameraMatrix");
			m_Material->UploadMat4Data("u_CameraMatrix", Renderer::Get()->GetCamera().GetViewProjection());
			m_Material->UploadMat4Data("u_Transform", m_Transform.mat4());

			glBindVertexArray(m_Mesh->GetVertexArrayObject());

			glDrawElements(GL_POLYGON, (GLsizei)m_Mesh->GetIndices().size(), GL_UNSIGNED_INT, 0);
		}

		/*if (b_ShowVertices)
		{
			Shader point_shader(Paths::Get("PointShader"));

			point_shader.Use();

			glBindVertexArray(m_VertexArrayObject);
			glDrawArrays(GL_POINTS, 0, m_BufferData.size() / 7);
			glUseProgram(0);
		}

		if (b_ShowLines)
		{
			Shader line_shader(Paths::Get("LineShader"));

			line_shader.Use();

			glBindVertexArray(m_VertexArrayObject);

			glDrawArrays(GL_LINE_LOOP, 0, m_BufferData.size() / 7);
			glUseProgram(0);
		}*/
	}
}