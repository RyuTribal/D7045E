#include "Cone.h"

namespace Lab4 {
    Cone::Cone(float radius, float height, unsigned int sectors)
    {
        std::vector<Engine::Vertex> vertices;
        std::vector<int> indices;

        float sectorStep = 2 * glm::pi<float>() / sectors;
        float sectorAngle;

        glm::vec3 apex(0, height, 0);
        vertices.push_back(Engine::Vertex(apex));
        vertices[0].normal = glm::normalize(apex);

        glm::vec3 baseCenter(0, 0, 0);
        vertices.push_back(Engine::Vertex(baseCenter));
        vertices[1].normal = glm::vec3(0, -1, 0);

        for (unsigned int i = 0; i <= sectors; ++i) {
            sectorAngle = i * sectorStep;
            float x = radius * cosf(sectorAngle);
            float z = radius * sinf(sectorAngle);

            glm::vec3 position(x, 0, z);
            glm::vec3 normal = glm::normalize(glm::vec3(x, radius, z));

            vertices.push_back(Engine::Vertex(position));
            vertices[vertices.size() - 1].normal = normal;
        }

        for (unsigned int i = 2; i <= sectors; ++i) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 2);
        }

        for (unsigned int i = 3; i <= sectors; ++i) {
            indices.push_back(1);
            indices.push_back(i + 2);
            indices.push_back(i);
        }

        SetData(vertices, indices);


    }
}