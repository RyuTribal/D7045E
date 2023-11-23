#include "pch.h"
#include "Monochrome.h"
#include <glad/gl.h>

namespace Engine{
	Monochrome::Monochrome(int red, int green, int blue) : r{ (float)red / 255.f }, g{ (float)green / 255.f }, b{ (float)blue / 255.f }
	{
		m_Program = CreateRef<ShaderProgram>(std::string(ROOT_PATH)+"/shaders/monochrome_shader");
	}
	void Monochrome::ApplyMaterial()
	{
		m_Program->Activate();

		int location = glGetUniformLocation(m_Program->GetProgram(), "u_MaterialColor");

		if (location != -1) {
			glUniform3f(location, r, g, b);
		}
		else {
			THROW_CORE_ERROR("Uniform 'u_MaterialColor' not found in Monochrome material");
		}
	}
}