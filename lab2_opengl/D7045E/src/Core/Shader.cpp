#include "pch.h"
#include "Shader.h"


namespace D7045E
{
	Shader::Shader(const std::string& path, GLenum type)
	{
        std::ifstream shaderFile(path);

        CORE_ASSERT(shaderFile.is_open(), "Failed to open Shader file")

        std::stringstream buffer;
        buffer << shaderFile.rdbuf();

        std::string shaderCode = buffer.str();

        shaderFile.close();

        GLuint shader = glCreateShader(type);
        const GLchar* shaderCodeCStr = shaderCode.c_str();

        glShaderSource(shader, 1, &shaderCodeCStr, NULL);


        glCompileShader(shader);

        GLint success;
        GLchar infoLog[1024];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            glDeleteShader(shader);
            THROW_CORE_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {0} \n{1}", type, infoLog);
        }

        m_Shader = shader;
	}
    Shader::~Shader()
    {
        glDeleteShader(m_Shader);
    }
}