#pragma once
#include "Renderer/Material.h"

namespace Engine {
	class DebugPoints : public Material {
	public:
		DebugPoints();

		void ApplyMaterial() override;

		std::string& GetName() override { return c_Name; }

	private:
		float r, g, b;
		std::string c_Name = "Vertices";
	};
}