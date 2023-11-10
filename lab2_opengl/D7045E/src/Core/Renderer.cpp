#include "pch.h"
#include "Renderer.h"

#include "GlobalState.h"
#include "glad/gl.h"
#include "Utils/Random.h"
#include "Core/Application.h"
#include "glm/gtx/io.hpp"

namespace D7045E
{
    Renderer* Renderer::s_Instance = nullptr;

    Renderer::Renderer(std::string vertexSrc, std::string fragSrc)
        : shaderProgram(glCreateProgram()), vertShader(Shader(vertexSrc, GL_VERTEX_SHADER)), fragShader(Shader(fragSrc, GL_FRAGMENT_SHADER)) {

        glAttachShader(shaderProgram, vertShader.GetSource());
        glAttachShader(shaderProgram, fragShader.GetSource());

        glLinkProgram(shaderProgram);

        glEnable(GL_PROGRAM_POINT_SIZE);

        GLint success;
        GLchar infoLog[1024];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            CORE_ERROR("Shader Linking Error: {0}", infoLog);
            return;
        }

        glCreateVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glCreateBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Renderer::~Renderer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }

    void Renderer::InitData() {
        vertices.clear();
        int count = GlobalState::Instance().POINT_COUNT >= 3 ? GlobalState::Instance().POINT_COUNT : 3;
        for (int i = 0; i < count; i++) {
            float x = Random::RandomFloat(-1.f, 1.f);
            float y = Random::RandomFloat(-1.f, 1.f);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0f);

            
        }
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Renderer::CreateTriangles()
    {
        // Here goes the triangle algorithm


    }

    void Renderer::PrepareFrame(float dt) {
        if(GlobalState::Instance().POINT_COUNT * 3 != vertices.size() && GlobalState::Instance().POINT_COUNT >=3)
        {
            InitData();
        }

        if (GlobalState::Instance().shouldDrawTriangles)
        {
            CreateTriangles();
        }

        glUseProgram(shaderProgram);
    }

    void Renderer::Draw() {
        float r, g, b;
        r = GlobalState::Instance().BG_COLOR[0] / 255.0f;
        g = GlobalState::Instance().BG_COLOR[1] / 255.0f;
        b = GlobalState::Instance().BG_COLOR[2] / 255.0f;

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        if(GlobalState::Instance().shouldDrawTriangles)
        {
            glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
            glDrawArrays(GL_LINE_LOOP, 0, vertices.size() / 3);
        }
        glDrawArrays(GL_POINTS, 0, vertices.size() / 3);
        glBindVertexArray(0);

        glUseProgram(0);
    }
}
