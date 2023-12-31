#include "pch.h"
#include "RenderContext.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace Engine
{
	RenderContext::RenderContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CORE_ASSERT(windowHandle, "Window handle is null!")
	}
	void RenderContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
		CORE_ASSERT(status, "Failed to initialize Glad!");

		glEnable(GL_DEBUG_OUTPUT);
	}
	void RenderContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
	std::unique_ptr<RenderContext> RenderContext::Create(void* window)
	{
		return std::make_unique<RenderContext>(std::forward<GLFWwindow*>(static_cast<GLFWwindow*>(window)));
	}
}