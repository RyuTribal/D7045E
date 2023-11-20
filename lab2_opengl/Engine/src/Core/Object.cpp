#include "pch.h"
#include "Object.h"

namespace Engine
{
	Object::Object(Ref<Shader> shader)
		: m_Shader(shader)
	{
		
	}
	Object::~Object()
	{
		glDeleteVertexArrays(1, &m_VertexArrayObject);
		glDeleteBuffers(1, &m_VertexArrayObject);
	}
	void Object::SetGLDrawType(GLuint type)
	{
		m_DrawType = type;
	}

	void Object::AddVertices(Ref<std::vector<Ref<Vertex>>> vertices)
	{
		m_Vertices = vertices;
	}

	void Object::Bind()
	{
		m_BufferData.clear();

		glCreateVertexArrays(1, &m_VertexArrayObject);
		glBindVertexArray(m_VertexArrayObject);

		glCreateBuffers(1, &m_VertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);

		for (auto vertex : *m_Vertices)
		{
			m_BufferData.push_back(vertex->x);
			m_BufferData.push_back(vertex->y);
			m_BufferData.push_back(vertex->z);

			m_BufferData.push_back(vertex->r);
			m_BufferData.push_back(vertex->g);
			m_BufferData.push_back(vertex->b);
			m_BufferData.push_back(vertex->a);
		}
		GLsizei stride = 7 * sizeof(float); // 3 for position + 4 for color, in bytes
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, m_BufferData.size() * sizeof(float), m_BufferData.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void Object::Draw()
	{

		if (b_ShowFaces) {
			m_Shader->Use();

			glBindVertexArray(m_VertexArrayObject);

			glDrawArrays(m_DrawType, 0, m_BufferData.size() / 7);
			glUseProgram(0);
		}

		if (b_ShowVertices) 
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
		}
	}
}