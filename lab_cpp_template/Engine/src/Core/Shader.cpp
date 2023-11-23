#include "pch.h"
#include "Shader.h"


namespace Engine
{
	Shader::Shader(const std::string& path) : m_ShaderProgram(glCreateProgram())
	{
        CreateShader(m_VertexShader ,path+".vert", GL_VERTEX_SHADER);
        CreateShader(m_FragmentShader, path + ".frag", GL_FRAGMENT_SHADER);
        glAttachShader(m_ShaderProgram, m_VertexShader);
        glAttachShader(m_ShaderProgram, m_FragmentShader);
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

    Shader::~Shader()
    {
        glDeleteProgram(m_ShaderProgram);
    }

    void Shader::CreateShader(GLuint& shader_ref, const std::string& path, GLuint type) {
        std::ifstream shaderFile(path);
        CORE_ASSERT(shaderFile.is_open(), "Failed to open Shader file");

        std::stringstream buffer;
        buffer << shaderFile.rdbuf();
        std::string shaderCode = buffer.str();
        shaderFile.close();

        shader_ref = glCreateShader(type);
        const GLchar* shaderCodeCStr = shaderCode.c_str();
        glShaderSource(shader_ref, 1, &shaderCodeCStr, NULL);
        glCompileShader(shader_ref);

        GLint success;
        glGetShaderiv(shader_ref, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shader_ref, sizeof(infoLog), NULL, infoLog);
            glDeleteShader(shader_ref);
            shader_ref = 0;
            CORE_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {} \n{}", type, infoLog);
        }
    }

    void Shader::Use()
    {
        glUseProgram(m_ShaderProgram);
    }

    void ShaderLibrary::Add(const std::string& name, std::string& file_path)
    {
        CORE_ASSERT(!Exists(name), "Shader already exists!");
        m_Shaders[name] = CreateRef<Shader>(file_path);
    }
    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        CORE_ASSERT(Exists(name), "Shader does not exist!");
        return m_Shaders[name];
    }
    bool ShaderLibrary::Exists(const std::string& name)
    {
        return m_Shaders.find(name) != m_Shaders.end();
    }
}