#pragma once
#include "Renderer/Material.h"

namespace Engine {
	class DebugLines : public Material {
	public:
		DebugLines();

		void ApplyMaterial() override;

		std::string& GetName() override { return c_Name; }

	private:
		float r, g, b;
		std::string c_Name = "Edges";
	};
}