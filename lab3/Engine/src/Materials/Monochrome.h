#pragma once
#include "Renderer/Material.h"

namespace Engine {
	class Monochrome : public Material {
	public:
		Monochrome(int red, int green, int blue);

		void ApplyMaterial() override;

		std::string& GetName() override { return c_Name; }

	private:
		float r, g, b;
		std::string c_Name = "Monochrome";
	};
}