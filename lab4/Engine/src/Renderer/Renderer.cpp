#include "pch.h"
#include "Renderer.h"
#include "Core/Application.h"

namespace Engine
{
    Renderer* Renderer::s_Instance = nullptr;

    Renderer::Renderer() : m_CurrentCamera(Camera(CameraType::PERSPECTIVE))
    {
    }

    Renderer::~Renderer() {
        
    }

    void Renderer::BeginFrame()
    {
        float r, g, b;
        r = m_BackgroundColor[0] / 255.0f;
        g = m_BackgroundColor[1] / 255.0f;
        b = m_BackgroundColor[2] / 255.0f;

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::Draw(Ref<GraphicsNode> node)
	{
        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    	node->Draw();
    }

    void Renderer::EndFrame()
    {
        glUseProgram(0);
        glBindVertexArray(0);
    }
}
