#include "pch.h"
#include "Monochrome.h"
#include "Renderer/Renderer.h"
#include <glad/gl.h>

namespace Engine{
	Monochrome::Monochrome(int red, int green, int blue) : r{ (float)red / 255.f }, g{ (float)green / 255.f }, b{ (float)blue / 255.f }
	{
		m_Program = CreateRef<ShaderProgram>(std::string(ROOT_PATH)+"/shaders/monochrome_shader");
	}
	void Monochrome::ApplyMaterial()
	{
		m_Program->Activate();

		UploadVec3Data("u_CameraPos", Renderer::Get()->GetCamera()->GetPosition());
		UploadVec3Data("u_MaterialColor", glm::vec3(r, g, b));
	}
}