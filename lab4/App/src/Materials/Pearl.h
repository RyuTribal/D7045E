#pragma once
#include <Engine.h>

namespace Lab4 {
	class Pearl : public Engine::Phong {
	public:
		Pearl() : Engine::Phong() {
			// Values taken from http://devernay.free.fr/cours/opengl/materials.html
			SetAmbient(glm::vec3(0.25f, 0.20725f, 0.20725f));
			SetDiffuse(glm::vec3(1.f, 0.829f, 0.829f));
			SetSpecular(glm::vec3(0.296648f, 0.296648f, 0.296648f));
			SetShininess(0.088f * 128);
		}
	};
}