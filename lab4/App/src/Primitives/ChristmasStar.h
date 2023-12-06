#pragma once
#include <Engine.h>

namespace Lab4 {
	class ChristmasStar : public Engine::Mesh {
	public:
		ChristmasStar(float outerDist, float innerDist, float thickness, unsigned int n);
	};
}