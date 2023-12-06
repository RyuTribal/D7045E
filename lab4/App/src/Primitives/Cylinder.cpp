#include "Cylinder.h"

namespace Lab4 {
	Cylinder::Cylinder(float radius, float height, unsigned int sectors)
	{
        std::vector<int> indices{};
        std::vector<Engine::Vertex> vertices = {};
        
        for (int i = 0; i <= sectors; ++i) {
            float angle = 2 * glm::pi<float>() * i / sectors;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            glm::vec3 normal = glm::normalize(glm::vec3(x, 0, z));

            vertices.push_back(Engine::Vertex({ x, height / 2, z }));

            vertices[vertices.size() -1].normal = normal;

            vertices.push_back(Engine::Vertex({ x, -height / 2, z }));

            vertices[vertices.size() - 1].normal = normal;
        }

        for (int i = 0; i < sectors; ++i) {
            indices.push_back(vertices.size());
            indices.push_back(i * 2);
            indices.push_back(((i + 1) % sectors) * 2);
        }

        for (int i = 0; i < sectors; ++i) {
            indices.push_back(vertices.size() + 1);
            indices.push_back(((i + 1) % sectors) * 2 + 1);
            indices.push_back(i * 2 + 1);
        }


        for (int i = 0; i < sectors; ++i) {
            int top1 = i * 2;
            int top2 = (i + 1) * 2 % (sectors * 2);
            int bottom1 = top1 + 1;
            int bottom2 = top2 + 1;

            indices.push_back(top1);
            indices.push_back(bottom1);
            indices.push_back(top2);

            indices.push_back(bottom1);
            indices.push_back(bottom2);
            indices.push_back(top2);
        }

        vertices.push_back(Engine::Vertex(glm::vec3(0.f, height / 2, 0.f)));
        vertices.push_back(Engine::Vertex(glm::vec3(0.f, -height / 2, 0.f))); 
      
        
        SetData(vertices, indices);


	}
}