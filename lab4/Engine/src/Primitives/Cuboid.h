#pragma once
#include "Renderer/Mesh.h"

namespace Engine {
	class Cuboid : public Mesh {
	public:
		Cuboid(float height, float width, float depth);

	private:
		float h, w, d;
	};
}