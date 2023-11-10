#pragma once

#include <glad/gl.h>

#include "Shader.h"

namespace D7045E
{

	struct GLFWwindow;

	class Renderer
	{
	public:
		Renderer(std::string vertexSrc, std::string fragSrc);
		~Renderer();

		void InitData();

		void SetupShaders();

		void PrepareFrame(float dt);

		void Draw();

		void CreateTriangles();

		static void CreateRenderer(std::string vertexSrc, std::string fragSrc)
		{
			if(!s_Instance)
			{
				s_Instance = new Renderer(vertexSrc, fragSrc);
			}
		}

		static Renderer* Get() { return s_Instance; }


	private:
		std::vector<float> vertices = {};

		GLuint VBO;

		float last_time_frame = 0.f;

		unsigned int shaderProgram;
		Shader vertShader;
		Shader fragShader;

		GLuint VAO;

		static Renderer* s_Instance;
	};



}
