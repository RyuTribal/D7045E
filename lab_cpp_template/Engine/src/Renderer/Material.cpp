#include "pch.h"
#include "Material.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {
	void Material::UploadMat4Data(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_Program->GetProgram(), name.c_str());
		if (location != -1) {
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
		}
		else {
			std::string error_message = "Shader '" + GetName() + "' does not have a uniform " + name + "!";
			THROW_CORE_ERROR(error_message);
		}

	}
	void Material::UploadVec3Data(const std::string& name, const glm::vec3& vector)
	{
		GLint location = glGetUniformLocation(m_Program->GetProgram(), name.c_str());
		if (location != -1) {
			glUniform3fv(location, 1, glm::value_ptr(vector));
		}
		else {
			std::string error_message = "Shader '" + GetName() + "' does not have a uniform " + name + "!";
			THROW_CORE_ERROR(error_message);
		}
	}
}