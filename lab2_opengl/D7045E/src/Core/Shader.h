#pragma once
#include <glad/gl.h>

namespace D7045E
{
	class Shader
	{
	public:
		Shader(const std::string& path, GLenum type);
		~Shader();

		GLuint GetSource() { return m_Shader; }

	private:
		GLuint m_Shader;
	};
}