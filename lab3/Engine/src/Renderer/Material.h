#pragma once
#include "ShaderProgram.h"

namespace Engine {
	class Material {
	public:
		virtual void ApplyMaterial() = 0;
		virtual std::string& GetName() = 0;

		Ref<ShaderProgram> GetProgram() { return m_Program; }

		void UploadMat4Data(const std::string& name, const glm::mat4& matrix);

	protected:
		Ref<ShaderProgram> m_Program;
	};
}