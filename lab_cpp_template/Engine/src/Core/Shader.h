#pragma once
#include <glad/gl.h>

namespace Engine
{
	class Shader
	{
	public:
		Shader(const std::string& path);
		~Shader();

		void CreateShader(GLuint& shader_ref, const std::string& path, GLuint type);

		void Use();
		
		GLuint GetProgram() { return m_ShaderProgram; }

	private:
		GLuint m_VertexShader;
		GLuint m_FragmentShader;
		GLuint m_ShaderProgram;
	};


	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, std::string& file_path);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}