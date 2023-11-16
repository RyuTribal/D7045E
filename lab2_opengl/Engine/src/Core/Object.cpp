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
	void Object::AddGLDrawType(GLuint type)
	{
		if (!DrawTypeExists(type)) {
			m_DrawTypes.push_back(type);
		}
	}

	void Object::RemoveGLDrawType(GLuint type)
	{
		for (size_t i = 0; i < m_DrawTypes.size(); i++)
		{
			if (type == m_DrawTypes[i])
			{
				m_DrawTypes.erase(m_DrawTypes.begin() + i);
				break;
			}
		}
	}


	bool Object::DrawTypeExists(GLuint type)
	{
		for(GLuint& existing_type: m_DrawTypes)
		{
			if(type == existing_type)
			{
				return true;
			}
		}
		return false;
	}

	void Object::AddVertices(Ref<std::vector<Ref<Vertex>>> vertices)
	{
		m_Vertices = vertices;

		glCreateVertexArrays(1, &m_VertexArrayObject);
		glBindVertexArray(m_VertexArrayObject);

		glCreateBuffers(1, &m_VertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Object::Bind()
	{
		m_BufferData.clear();
		for (auto vertex : *m_Vertices)
		{
			m_BufferData.push_back(vertex->x);
			m_BufferData.push_back(vertex->y);
			m_BufferData.push_back(vertex->z);
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, m_BufferData.size() * sizeof(float), m_BufferData.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Object::Draw()
	{
		m_Shader->Use();


		glBindVertexArray(m_VertexArrayObject);
		for(GLuint drawType : m_DrawTypes)
		{
			glDrawArrays(drawType, 0, m_BufferData.size() / 3);
		}
		m_DrawTypes.clear();
	}


}