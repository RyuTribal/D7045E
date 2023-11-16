#include "pch.h"
#include "Renderer.h"

namespace Engine
{
    Renderer* Renderer::s_Instance = nullptr;

    Renderer::Renderer()
    {
        
    }

    Renderer::~Renderer() {
        
    }

    void Renderer::Draw(Ref<Object> object)
	{
        glEnable(GL_PROGRAM_POINT_SIZE);
        object->Bind();
    	object->Draw();
    }

    void Renderer::EndFrame()
    {
        glUseProgram(0);
        glBindVertexArray(0);
    }



    void Renderer::SetClearColor(const int* rgb)
    {
        float r, g, b;
        r = rgb[0] / 255.0f;
        g = rgb[1] / 255.0f;
        b = rgb[2] / 255.0f;

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
