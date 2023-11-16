#pragma once

#include <glad/gl.h>

#include "Shader.h"


namespace Engine
{
	class Object
	{
	public:
		Object(Ref<Shader> shader);
		~Object();

		void AddGLDrawType(GLuint type);
		void RemoveGLDrawType(GLuint type);
		bool DrawTypeExists(GLuint type);

		void AddVertices(Ref<std::vector<Ref<Vertex>>> vertices);
		Ref<std::vector<Ref<Vertex>>> GetVertices() { return m_Vertices; }

		void Bind();
		void Draw();

	private:
		std::vector<float> m_BufferData = {};
		Ref<std::vector<Ref<Vertex>>> m_Vertices = CreateRef<std::vector<Ref<Vertex>>>();
		std::vector<GLuint> m_DrawTypes;
		GLuint m_VertexBufferObject;
		GLuint m_VertexArrayObject;
		Ref<Shader> m_Shader;
	};
}
