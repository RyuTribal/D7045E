#include "pch.h"
#include "Cuboid.h"

namespace Engine {
	Cuboid::Cuboid(float height, float width, float depth) : h(height), w(width), d(depth)
	{
        float half_width = w / 2.0f;
        float half_height = h / 2.0f;
        float half_depth = d / 2.0f;

        // Define vertices
        std::vector<Vertex> vertices = {
            // Front face
            Vertex(-half_width, -half_height, half_depth),  // 0: Bottom Left
            Vertex(half_width, -half_height, half_depth),   // 1: Bottom Right
            Vertex(half_width,  half_height, half_depth),   // 2: Top Right
            Vertex(-half_width,  half_height, half_depth),  // 3: Top Left

            // Back face
            Vertex(-half_width, -half_height, -half_depth), // 4: Bottom Left
            Vertex(half_width, -half_height, -half_depth),  // 5: Bottom Right
            Vertex(half_width,  half_height, -half_depth),  // 6: Top Right
            Vertex(-half_width,  half_height, -half_depth)  // 7: Top Left
        };

        // Define indices
        std::vector<int> indices = {
            // Front face
            0, 1, 2, 2, 3, 0,
            // Back face
            5, 4, 7, 7, 6, 5,
            // Top face
            3, 2, 6, 6, 7, 3,
            // Bottom face
            4, 5, 1, 1, 0, 4,
            // Right face
            1, 5, 6, 6, 2, 1,
            // Left face
            4, 0, 3, 3, 7, 4
        };

        SetData(vertices, indices);
	}
}