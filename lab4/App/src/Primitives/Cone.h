#pragma once
#include <Engine.h>

namespace Lab4 {
	class Cone : public Engine::Mesh {
	public:
		Cone(float radius, float height, unsigned int sectors);
	};
}