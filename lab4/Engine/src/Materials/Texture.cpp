#include "pch.h"
#include "Texture.h"
#include <glad/gl.h>

namespace Engine {
	Texture::Texture(GLuint texture) : m_Texture{texture}
	{
		std::string path = std::string(ROOT_PATH) + "/shaders/texture_shader";
		m_Program = CreateRef<ShaderProgram>(path);
	}
	void Texture::ApplyMaterial()
	{
		m_Program->Activate();
	}
}