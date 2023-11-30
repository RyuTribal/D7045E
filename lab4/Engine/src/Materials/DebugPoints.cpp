#include "pch.h"
#include "DebugPoints.h"

namespace Engine {
	DebugPoints::DebugPoints()
	{
		std::string path = std::string(ROOT_PATH) + "/shaders/point_shader";
		m_Program = CreateRef<ShaderProgram>(path);
	}
	void DebugPoints::ApplyMaterial()
	{
		m_Program->Activate();
	}
}