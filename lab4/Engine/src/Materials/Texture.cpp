#include "pch.h"
#include "Texture.h"
#include <glad/gl.h>

namespace Engine {
	Texture::Texture(GLuint texture) : m_Texture{texture}
	{
		m_Program = CreateRef<ShaderProgram>(std::string(ROOT_PATH) + "/shaders/texture_shader");
	}
	void Texture::ApplyMaterial()
	{
		m_Program->Activate();
	}
}