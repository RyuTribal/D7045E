#pragma once
#include <Engine.h>

namespace Lab4 {
	class Gold : public Engine::Phong {
	public:
		Gold() : Engine::Phong() {
			// Values taken from http://devernay.free.fr/cours/opengl/materials.html
			SetAmbient(glm::vec3(0.24725f, 0.1995f, 0.0745f));
			SetDiffuse(glm::vec3(0.75164f, 0.60648f, 0.22648f));
			SetSpecular(glm::vec3(0.628281f, 0.555802f, 0.366065f));
			SetShininess(0.4f * 128);
		}
	};
}