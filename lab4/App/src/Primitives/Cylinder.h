#pragma once
#include <Engine.h>

namespace Lab4 {
	class Cylinder : public Engine::Mesh {
	public:
		Cylinder(float radius, float height, unsigned int sectors);
	};
}