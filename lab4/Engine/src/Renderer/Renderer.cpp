#include "pch.h"
#include "Renderer.h"
#include "Core/Application.h"

#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Engine
{
    Renderer* Renderer::s_Instance = nullptr;

    Renderer::Renderer()
    {
    }

    Renderer::~Renderer() {
        
    }

    void Renderer::BeginFrame(Camera* camera)
    {
        float r, g, b;
        r = m_BackgroundColor[0] / 255.0f;
        g = m_BackgroundColor[1] / 255.0f;
        b = m_BackgroundColor[2] / 255.0f;

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        m_CurrentCamera = camera;
        camera->UpdateCamera();
    }

    void Renderer::EndFrame()
    {
        glUseProgram(0);
        glBindVertexArray(0);
    }


	TextureInfo Renderer::UploadImageToGPU(const char* path, bool invert)
	{
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(invert);
		unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
		if (data)
		{
			if (nrChannels == 4) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			else if (nrChannels == 3) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			CORE_ERROR("Failed to load texture");
			return TextureInfo();
		}
		stbi_image_free(data);

		TextureInfo texture_info{};
		texture_info.texture = texture;
		texture_info.height = height;
		texture_info.width = width;
		texture_info.channel_number = nrChannels;

		return texture_info;
	}
}
