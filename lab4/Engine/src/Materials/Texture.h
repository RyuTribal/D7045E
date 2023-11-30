#pragma once
#include "Renderer/Material.h"

namespace Engine {
	class Texture : public Material {
	public:
		Texture(GLuint texture);

		void ApplyMaterial() override;

		std::string& GetName() override { return c_Name; }

	private:
		GLuint m_Texture;
		std::string c_Name = "Texture";
	};
}
