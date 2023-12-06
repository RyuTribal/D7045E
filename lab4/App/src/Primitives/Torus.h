#pragma once
#include <Engine.h>

namespace Lab4 {
	class Torus : public Engine::Mesh {
	public:
		Torus(float radius, float tubeRadius, unsigned int radialSegments, unsigned int tubularSegments);
	};
}