#pragma once
#include "Shader.h"

namespace Engine {
	class ShaderProgram {
	public:
		ShaderProgram(std::string& path);
		~ShaderProgram();
		GLuint GetProgram() { return m_ShaderProgram; }

		void Activate();
	private:
		GLuint m_ShaderProgram;
		Shader m_VertexShader;
		Shader m_FragmentShader;
	};
}