#include "pch.h"
#include "ShaderProgram.h"

namespace Engine {
	ShaderProgram::ShaderProgram(const std::string& path) : m_VertexShader(Shader(path + ".vert", GL_VERTEX_SHADER)), m_FragmentShader(Shader(path + ".frag", GL_FRAGMENT_SHADER))
	{
        m_ShaderProgram = glCreateProgram();
        glAttachShader(m_ShaderProgram, m_VertexShader.Handle());
        glAttachShader(m_ShaderProgram, m_FragmentShader.Handle());
        glLinkProgram(m_ShaderProgram);

        GLint success;
        GLchar infoLog[1024];
        glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
            CORE_ERROR("Shader Linking Error: {0}", infoLog);
            return;
        }
	}
    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(m_ShaderProgram);
    }
    void ShaderProgram::Activate()
    {
        glUseProgram(m_ShaderProgram);
    }
}