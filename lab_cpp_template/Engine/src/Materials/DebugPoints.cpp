#include "pch.h"
#include "DebugPoints.h";

namespace Engine {
	DebugPoints::DebugPoints()
	{
		m_Program = CreateRef<ShaderProgram>(std::string(ROOT_PATH) + "/shaders/point_shader");
	}
	void DebugPoints::ApplyMaterial()
	{
		m_Program->Activate();
	}
}