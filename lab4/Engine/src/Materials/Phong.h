#pragma once
#include "Renderer/Material.h"

namespace Engine {
	class Phong : public Material {
	public:
		Phong();

		std::string& GetName() override { return c_Name; }

		void ApplyMaterial() override;

		glm::vec3& GetDiffuse() { return m_Diffuse; }
		glm::vec3& GetAmbient() { return m_Ambient; }
		glm::vec3& GetSpecular() { return m_Specular; }
		float GetShininess() { return m_Shininess; }

		void SetDiffuse(glm::vec3& diffuse) { m_Diffuse = diffuse; }
		void SetAmbient(glm::vec3& ambient) { m_Ambient = ambient; }
		void SetSpecular(glm::vec3& specular) { m_Specular = specular; }
		void SetShininess(float shininess) { m_Shininess = shininess; }

	private:
		std::string c_Name = "Phong";

		glm::vec3 m_Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec3 m_Ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		glm::vec3 m_Specular = glm::vec3(1.f, 1.f, 1.f);
		float m_Shininess;
	};
}