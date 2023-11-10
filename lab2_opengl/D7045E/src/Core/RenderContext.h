#pragma once

struct GLFWwindow;

namespace D7045E {

	class RenderContext
	{
	public:
		RenderContext(GLFWwindow* windowHandle);

		void Init();
		void SwapBuffers();

		static std::unique_ptr<RenderContext> Create(void* window);

	private:


		GLFWwindow* m_WindowHandle;
	};
}