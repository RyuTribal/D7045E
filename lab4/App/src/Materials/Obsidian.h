#pragma once
#include <Engine.h>

namespace Lab4 {
	class Obsidian : public Engine::Phong {
	public:
		Obsidian() : Engine::Phong() {
			// Values taken from http://devernay.free.fr/cours/opengl/materials.html
			SetAmbient(glm::vec3(0.05375f, 0.05f, 0.06625f));
			SetDiffuse(glm::vec3(0.18275f, 0.17f, 0.22525f));
			SetSpecular(glm::vec3(0.332741f, 0.328634f, 0.346435f));
			SetShininess(0.3f * 128);
		}
	};
}