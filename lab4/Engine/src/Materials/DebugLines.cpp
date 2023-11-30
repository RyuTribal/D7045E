#include "pch.h"
#include "DebugLines.h"

namespace Engine {
	DebugLines::DebugLines()
	{
		std::string path = std::string(ROOT_PATH) + "/shaders/line_shader";
		m_Program = CreateRef<ShaderProgram>(path);
	}
	void DebugLines::ApplyMaterial()
	{
		m_Program->Activate();
	}
}