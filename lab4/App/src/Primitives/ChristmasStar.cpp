#include "ChristmasStar.h"

namespace Lab4 {
    ChristmasStar::ChristmasStar(float outerDist, float innerDist, float thickness, unsigned int n) {
        std::vector<Engine::Vertex> vertices;
        std::vector<int> indices;

        // Check for valid parameters
        if (n <= 2 || thickness <= 0 || outerDist > innerDist) {
            THROW_CORE_ERROR("That aint a christmas star and you know it!");
            return;
        }

        // Center vertices
        vertices.push_back(Engine::Vertex(glm::vec3(0, 0, thickness / 2)));  // Front center
        vertices[vertices.size() - 1].normal = glm::normalize(vertices[vertices.size() - 1].coordinates);
        vertices.push_back(Engine::Vertex(glm::vec3(0, 0, -thickness / 2))); // Back center
        vertices[vertices.size() - 1].normal = glm::normalize(vertices[vertices.size() - 1].coordinates);


        // Calculate outer and inner vertices
        for (unsigned int i = 0; i < n; ++i) {
            float angle = 2 * glm::pi<float>() * i / n;
            float xOuter = outerDist * sin(angle);
            float yOuter = outerDist * cos(angle);
            float xInner = innerDist * sin(angle + glm::pi<float>() / n);
            float yInner = innerDist * cos(angle + glm::pi<float>() / n);

            vertices.push_back(Engine::Vertex(glm::vec3(xOuter, yOuter, 0))); // Outer vertex
            vertices[vertices.size() - 1].normal = glm::normalize(vertices[vertices.size() - 1].coordinates);
            vertices.push_back(Engine::Vertex(glm::vec3(xInner, yInner, 0))); // Inner vertex
            vertices[vertices.size() - 1].normal = glm::normalize(vertices[vertices.size() - 1].coordinates);
        }

        // Define indices
        for (unsigned int i = 0; i < n; ++i) {
            int outerIndex = 2 + i * 2;
            int innerIndex = 3 + i * 2;
            int nextOuterIndex = 2 + ((i + 1) % n) * 2;

            // Front side
            indices.push_back(0); // Front center vertex
            indices.push_back(outerIndex);
            indices.push_back(innerIndex);

            indices.push_back(0); // Front center vertex
            indices.push_back(innerIndex);
            indices.push_back(nextOuterIndex);

            // Back side
            indices.push_back(1); // Back center vertex
            indices.push_back(innerIndex);
            indices.push_back(outerIndex);

            indices.push_back(1); // Back center vertex
            indices.push_back(nextOuterIndex);
            indices.push_back(innerIndex);
        }

        SetData(vertices, indices);
    }
}