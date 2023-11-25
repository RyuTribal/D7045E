#include "pch.h"
#include "Cuboid.h"

namespace Engine {
	Cuboid::Cuboid(float height, float width, float depth) : h(height), w(width), d(depth)
	{
        std::vector<int> indices = {
            0, 1, 3, 3, 1, 2,
            1, 5, 2, 2, 5, 6,
            5, 4, 6, 6, 4, 7,
            4, 0, 7, 7, 0, 3,
            3, 2, 7, 7, 2, 6,
            4, 5, 0, 0, 5, 1
        };

        float half_width = w / 2.0f;
        float half_height = h / 2.0f;
        float half_depth = d / 2.0f;

        std::vector<Vertex> vertices = {
            Vertex(-half_width, -half_height, -half_depth),
            Vertex(half_width, -half_height, -half_depth),
            Vertex(half_width, half_height, -half_depth),
            Vertex(-half_width, half_height, -half_depth),
            Vertex(-half_width, -half_height, half_depth),
            Vertex(half_width, -half_height, half_depth),
            Vertex(half_width, half_height, half_depth),
            Vertex(-half_width, half_height, half_depth)
        };

        SetData(vertices, indices);
	}
}