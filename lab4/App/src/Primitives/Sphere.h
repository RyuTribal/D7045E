#pragma once
#include <Engine.h>

namespace Lab4 {
	class Sphere : public Engine::Mesh {
	public:
		Sphere(float radius, unsigned int sectors, unsigned int stacks);

	private:
		float h, w, d;
	};
}