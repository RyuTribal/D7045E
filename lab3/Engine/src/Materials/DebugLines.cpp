#include "pch.h"
#include "DebugLines.h";

namespace Engine {
	DebugLines::DebugLines()
	{
		m_Program = CreateRef<ShaderProgram>(std::string(ROOT_PATH) + "/shaders/line_shader");
	}
	void DebugLines::ApplyMaterial()
	{
		m_Program->Activate();
	}
}