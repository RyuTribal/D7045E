#pragma once

/*
 * This file is for simplistic data structures
 */

namespace Engine
{

	struct Vertex
	{
		float x;
		float y;
		float z;

		Vertex(float x, float y, float z) : x(x), y(y), z(z) {}
		Vertex() : x(0.f), y(0.f), z(0.f) {} // A default constructor is always a good idea

		bool operator==(const Vertex& other) const {
			return x == other.x && y == other.y && z == other.z;
		}
	};


    class Triangle {
    public:
        std::vector<Ref<Vertex>> vertices[3];
        std::vector<Ref<Triangle>> neighbors[3];

        Triangle(Ref<Vertex> v1, Ref<Vertex> v2,  Ref<Vertex> v3) {
            vertices->insert(vertices->begin() + 0, v1);
            vertices->insert(vertices->begin() + 1, v2);
            vertices->insert(vertices->begin() + 2, v3);
            neighbors->insert(neighbors->begin() + 0, nullptr);
            neighbors->insert(neighbors->begin() + 1, nullptr);
            neighbors->insert(neighbors->begin() + 2, nullptr);
        }

        void SetNeighbor(int edgeIndex, Ref<Triangle> neighbor) {
            if (edgeIndex >= 0 && edgeIndex < 3) {
                neighbors->insert(neighbors->begin() + edgeIndex, neighbor);
            }
        }

        int IndexOf(Ref<Triangle> neighbor) {
            for (int i = 0; i < 3; ++i) {
                if (neighbors->at(i) == neighbor) {
                    return i;
                }
            }
            return -1;
        }

        static float TriangleArea(Ref<Vertex> A, Ref<Vertex> B, Ref<Vertex> C) {
            return std::abs((A->x * (B->y - C->y) + B->x * (C->y - A->y) + C->x * (A->y - B->y)) / 2.0f);
        }

        static bool IsPointInTriangle(Ref<Vertex> P, Ref<Triangle> triangle) {

            float fullArea = TriangleArea(triangle->vertices->at(0), triangle->vertices->at(1), triangle->vertices->at(2));

            float area1 = TriangleArea(P, triangle->vertices->at(1), triangle->vertices->at(2));
            float area2 = TriangleArea(triangle->vertices->at(0), P, triangle->vertices->at(2));
            float area3 = TriangleArea(triangle->vertices->at(0), triangle->vertices->at(1), P);


            return std::abs(fullArea - (area1 + area2 + area3)) < 0.0001f;
        }

        static int SharedEdge(Ref<Triangle> t1, Ref<Triangle> t2) {
            int sharedVertices = 0;
            int lastFoundIndex = -1;

            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (t1->vertices[i] == t2->vertices[j]) {
                        sharedVertices++;
                        lastFoundIndex = i;
                        break;
                    }
                }
            }

            if (sharedVertices == 2) {
                return lastFoundIndex;
            }
            return -1;
        }


        static void FlattenTriangles(const std::vector<Ref<Triangle>>& triangles, std::vector<Ref<Vertex>>& flattened_verteces)
        {
	        for(auto triangle : triangles)
	        {
                flattened_verteces.push_back(triangle->vertices->at(0));
                flattened_verteces.push_back(triangle->vertices->at(1));
                flattened_verteces.push_back(triangle->vertices->at(2));
	        }
        }


    };
}