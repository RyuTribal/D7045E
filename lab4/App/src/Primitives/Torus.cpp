#include "Torus.h"

namespace Lab4 {
    Torus::Torus(float radius, float tubeRadius, unsigned int radialSegments, unsigned int tubularSegments)
    {
        std::vector<Engine::Vertex> vertices;
        std::vector<int> indices;

        float radialStep = 2 * glm::pi<float>() / radialSegments;
        float tubularStep = 2 * glm::pi<float>() / tubularSegments;
        float radialAngle, tubularAngle;

        for (unsigned int i = 0; i <= radialSegments; ++i) {
            radialAngle = i * radialStep;

            for (unsigned int j = 0; j <= tubularSegments; ++j) {
                tubularAngle = j * tubularStep;

                float x = (radius + tubeRadius * cosf(tubularAngle)) * cosf(radialAngle);
                float y = (radius + tubeRadius * cosf(tubularAngle)) * sinf(radialAngle);
                float z = tubeRadius * sinf(tubularAngle);

                glm::vec3 position(x, y, z);
                glm::vec3 normal = glm::normalize(glm::vec3(cosf(tubularAngle) * cosf(radialAngle), cosf(tubularAngle) * sinf(radialAngle), sinf(tubularAngle)));

                vertices.push_back(Engine::Vertex(position));
                vertices[vertices.size() - 1].normal = normal;
            }
        }

        unsigned int a, b, c, d;
        for (unsigned int i = 0; i < radialSegments; ++i) {
            for (unsigned int j = 0; j < tubularSegments; ++j) {
                a = i * (tubularSegments + 1) + j;
                b = i * (tubularSegments + 1) + (j + 1);
                c = (i + 1) * (tubularSegments + 1) + (j + 1);
                d = (i + 1) * (tubularSegments + 1) + j;

                indices.push_back(a);
                indices.push_back(b);
                indices.push_back(d);

                indices.push_back(b);
                indices.push_back(c);
                indices.push_back(d);
            }
        }

        SetData(vertices, indices);


    }
}