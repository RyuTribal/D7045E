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

		void SetGLDrawType(GLuint type);
		GLuint GetGLDrawType() { return m_DrawType; }

		void SetShowFaces(bool show_faces) { b_ShowFaces = show_faces; }
		void SetShowLines(bool show_lines) { b_ShowLines = show_lines; }
		void SetShowVertices(bool show_vertices) { b_ShowVertices = show_vertices; }

		void AddVertices(Ref<std::vector<Ref<Vertex>>> vertices);
		Ref<std::vector<Ref<Vertex>>> GetVertices() { return m_Vertices; }

		void Bind();
		void Draw();

	private:
		std::vector<float> m_BufferData = {};
		Ref<std::vector<Ref<Vertex>>> m_Vertices = CreateRef<std::vector<Ref<Vertex>>>();
		GLuint m_DrawType = GL_TRIANGLES;
		GLuint m_VertexBufferObject;
		GLuint m_VertexArrayObject;
		Ref<Shader> m_Shader;
		
		bool b_ShowFaces = true;
		bool b_ShowLines = false;
		bool b_ShowVertices = false;
	};
}
