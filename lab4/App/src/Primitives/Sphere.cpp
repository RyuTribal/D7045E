#include "Sphere.h"

namespace Lab4 {
	Sphere::Sphere(float radius, unsigned int sectors, unsigned int stacks) {
        std::vector<Engine::Vertex> vertices;
        std::vector<int> indices;

        float sectorStep = 2 * glm::pi<float>() / sectors;
        float stackStep = glm::pi<float>() / stacks;
        float sectorAngle, stackAngle;


        for (unsigned int i = 0; i <= stacks; ++i) {
            stackAngle = glm::pi<float>() / 2 - i * stackStep;
            float xy = radius * cosf(stackAngle);
            float z = radius * sinf(stackAngle);

            for (unsigned int j = 0; j <= sectors; ++j) {
                sectorAngle = j * sectorStep;

                Engine::Vertex vertex;
                vertex.coordinates.x = xy * cosf(sectorAngle);
                vertex.coordinates.y = xy * sinf(sectorAngle);
                vertex.coordinates.z = z; 
                glm::vec3 normal = glm::normalize(vertex.coordinates);
                vertex.normal = normal;
                vertices.push_back(vertex);

            }
        }

        unsigned int k1, k2;
        for (unsigned int i = 0; i < stacks; ++i) {
            k1 = i * (sectors + 1);
            k2 = k1 + sectors + 1;

            for (unsigned int j = 0; j < sectors; ++j, ++k1, ++k2) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stacks - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

        SetData(vertices, indices);
	}
}