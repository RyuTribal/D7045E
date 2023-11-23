#include "pch.h"
#include "Renderer.h"
#include "Core/Application.h"

namespace Engine
{
    Renderer* Renderer::s_Instance = nullptr;

    Renderer::Renderer() : m_CurrentCamera(Camera(glm::radians(45.f), static_cast<float>(Application::Get().GetWindow().GetWidth()) / Application::Get().GetWindow().GetHeight(), 0.1f, 1000.f))
    {
        
    }

    Renderer::~Renderer() {
        
    }

    void Renderer::BeginFrame(const int* rgb)
    {
        float r, g, b;
        r = rgb[0] / 255.0f;
        g = rgb[1] / 255.0f;
        b = rgb[2] / 255.0f;

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::Draw(Ref<GraphicsNode> node)
	{
        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_DEPTH_TEST);
    	node->Draw();
    }

    void Renderer::EndFrame()
    {
        glUseProgram(0);
        glBindVertexArray(0);
    }
}
