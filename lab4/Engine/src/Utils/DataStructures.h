#pragma once

/*
 * This file is for simplistic data structures
 */

namespace Engine
{

	struct Vertex
	{
        float x, y, z;
        float r = 1.0f;
        float g = 1.0f;
        float b = 1.0f;
        float a = 1.0f;

		Vertex(float x, float y, float z) : x(x), y(y), z(z) {}
		Vertex() : x(0.f), y(0.f), z(0.f) {} // A default constructor is always a good idea

        void SetColor(int red, int green, int blue) 
        {
            r = static_cast<float>(red) / 255.f;
            g = static_cast<float>(green) / 255.f;
            b = static_cast<float>(blue) / 255.f;
        }

        void SetColor(float red, float green, float blue)
        {
            r = red;
            g = green;
            b = blue;
        }

        void SetOpacity(float alpha) { a = alpha; }

        void TransformData(std::vector<float>& target_vector) {
            target_vector.push_back(x);
            target_vector.push_back(y);
            target_vector.push_back(z);

            target_vector.push_back(r);
            target_vector.push_back(g);
            target_vector.push_back(b);
            target_vector.push_back(a);
        }

        bool operator==(const Vertex& other) const {
            return std::abs(x - other.x) < EPSILON &&
                std::abs(y - other.y) < EPSILON &&
                std::abs(z - other.z) < EPSILON;
        }

        static float DistanceBetweenTwoVerts(const Vertex& v1, const Vertex v2) {
            return static_cast<float>(sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2)));
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

        int NeighborLength() {
            int length = 0;
            for (Ref<Triangle> neighbor : *neighbors) {
                if (neighbor != nullptr) {
                    length++;
                }
            }

            return length;
        }

        void SetNeighbor(int edgeIndex, Ref<Triangle> neighbor) {
            if (edgeIndex >= 0 && edgeIndex < 3) {
                neighbors->insert(neighbors->begin() + edgeIndex, neighbor);
            }
        }

        void SetColor(int r, int g, int b) 
        {
            for (Ref<Vertex>& vertex : *vertices) 
            {
                vertex->SetColor(r, g, b);
            }
        }

        void SetColor(float r, float g, float b)
        {
            for (Ref<Vertex>& vertex : *vertices)
            {
                vertex->SetColor(r, g, b);
            }
        }

        void SetOpacity(float alpha) 
        {
            for (Ref<Vertex>& vertex : *vertices)
            {
                vertex->SetOpacity(alpha);
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


        static void FlattenTriangles(const std::vector<Ref<Triangle>>& triangles, std::vector<Ref<Vertex>>& flattened_vertices)
        {
	        for(auto triangle : triangles)
	        {
                flattened_vertices.push_back(triangle->vertices->at(0));
                flattened_vertices.push_back(triangle->vertices->at(1));
                flattened_vertices.push_back(triangle->vertices->at(2));
	        }
        }


    };

    struct Edge {
        Ref<Vertex> v1, v2;
        Ref<Triangle> origin;

        Edge(Ref<Vertex> vertex1, Ref<Vertex> vertex2, Ref<Triangle> from_triangle) {
            v1 = vertex1;
            v2 = vertex2;
            origin = from_triangle;
        }

        bool operator==(const Edge& other) const {
            return (v1 == other.v1 && v2 == other.v2) ||
                (v1 == other.v2 && v2 == other.v1);
        }

        static int FindEdgeIndex(Ref<Edge> edge, Ref<Triangle> triangle) {
            for (int i = 0; i < 3; ++i) {
                Ref<Vertex> v1 = triangle->vertices->at(i);
                Ref<Vertex> v2 = triangle->vertices->at((i + 1) % 3);

                if ((*v1.get() == *edge->v1.get() && *v2.get() == *edge->v2.get()) ||
                    (*v1.get() == *edge->v2.get() && *v2.get() == *edge->v1.get())) {
                    return i;
                }
            }
            return -1;
        }
    };
}