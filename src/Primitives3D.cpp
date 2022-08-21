#include "Primitives3D.h"
#include "Macros.h"
#include "Primitives.h"

namespace algorithm {
    static std::unique_ptr<Mesh> CreateCuboidMesh(const Cuboid& rCuboid);
    static std::unique_ptr<Mesh> CreateSphereMesh(const Sphere& rSphere);

    std::unique_ptr<Mesh> PrimitiveToMesh(const Primitives& rPrimitive) {
        std::unique_ptr<Mesh> spMesh;
        switch (rPrimitive._type) {
        case Primitives::Types::Cuboid:
            spMesh = CreateCuboidMesh(dynamic_cast<const Cuboid&>(rPrimitive));
            break;
        case Primitives::Types::Sphere:
            spMesh = CreateSphereMesh(dynamic_cast<const Sphere&>(rPrimitive));
            break;
        default:
            break;
        }

        return spMesh;
    }

    std::unique_ptr<Mesh> CreateCuboidMesh(const Cuboid& rQuader) {
        std::vector<Eigen::Vector3f> vertices;
        vertices.reserve(8);
        std::vector<uint32_t> indices;
        indices.reserve(36);

        const float w = rQuader.GetWidth() * 0.5f;
        const float h = rQuader.GetHeight() * 0.5f;
        const float d = rQuader.GetDepth() * 0.5f;

        // front vertices
        vertices.emplace_back(Eigen::Vector3f(-w, h, -d));   // 0
        vertices.emplace_back(Eigen::Vector3f(w, h, -d));    // 1
        vertices.emplace_back(Eigen::Vector3f(w, -h, -d));   // 2
        vertices.emplace_back(Eigen::Vector3f(-w, -h, -d));  // 3

        // back vertices
        vertices.emplace_back(Eigen::Vector3f(w, h, d));    // 4
        vertices.emplace_back(Eigen::Vector3f(-w, h, d));   // 5
        vertices.emplace_back(Eigen::Vector3f(-w, -h, d));  // 6
        vertices.emplace_back(Eigen::Vector3f(w, -h, d));   // 7

        auto createTriangleIndices =
            [&indices](uint32_t upperLeft, uint32_t upperRight,
                       uint32_t lowerRight, uint32_t lowerLeft) {
                // First triangle
                indices.push_back(upperLeft);
                indices.push_back(lowerRight);
                indices.push_back(lowerLeft);

                // Second triangle
                indices.push_back(lowerRight);
                indices.push_back(upperLeft);
                indices.push_back(upperRight);
            };
        auto createTriangleIndicesReverse =
            [&indices](uint32_t upperLeft, uint32_t upperRight,
                       uint32_t lowerRight, uint32_t lowerLeft) {
                // First triangle
                indices.push_back(upperRight);
                indices.push_back(lowerLeft);
                indices.push_back(upperLeft);

                // Second triangle
                indices.push_back(upperRight);
                indices.push_back(lowerRight);
                indices.push_back(lowerLeft);
            };

        // front indices
        createTriangleIndices(0, 1, 2, 3);
        // right indices
        createTriangleIndicesReverse(1, 4, 7, 2);
        // back indices
        createTriangleIndices(4, 5, 6, 7);
        // left indices
        createTriangleIndicesReverse(5, 0, 3, 6);
        // bottom indices
        createTriangleIndicesReverse(3, 2, 7, 6);
        // top indices
        createTriangleIndicesReverse(5, 4, 1, 0);

        auto spMesh = std::make_unique<Mesh>();
        for (const uint32_t index : indices) {
            Vertex vertex;
            vertex.position = vertices[index];
            spMesh->vertices.emplace_back(vertex);
        }

        return spMesh;
    }

    std::unique_ptr<Mesh> CreateSphereMesh(const Sphere& rSphere) {
        float x, y, z, xy;                                         // vertex position
        float nx, ny, nz, lengthInv = 1.0f / rSphere.GetRadius();  // vertex normal
        const uint32_t _stackCount = 10;
        const uint32_t _sectorCount = 15;

        float sectorStep = static_cast<float>(2. * math::PI / _sectorCount);
        float stackStep = static_cast<float>(math::PI / _stackCount);
        float sectorAngle, stackAngle;

        std::vector<Vertex> vertices;
        vertices.reserve(_stackCount * _sectorCount);

        for (uint32_t i = 0; i <= _stackCount; ++i) {
            stackAngle = static_cast<float>(math::PI / 2.) - static_cast<float>(i) * stackStep;  // starting from pi/2 to -pi/2
            xy = rSphere.GetRadius() * cosf(stackAngle);                                         // r * cos(u)
            z = rSphere.GetRadius() * sinf(stackAngle);                                          // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (uint32_t j = 0; j <= _sectorCount; ++j) {
                Vertex vertex;
                sectorAngle = j * sectorStep;  // starting from 0 to 2pi

                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);  // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);  // r * cos(u) * sin(v)
                vertex.position = Eigen::Vector3f(x, y, z);

                // normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                vertex.normal = Eigen::Vector3f(nx, ny, nz);
            }
        }

        std::vector<uint32_t> indices;
        int k1, k2;
        for (uint32_t i = 0; i < _stackCount; ++i) {
            k1 = i * (_sectorCount + 1);  // beginning of current stack
            k2 = k1 + _sectorCount + 1;   // beginning of next stack

            for (uint32_t j = 0; j < _sectorCount; ++j, ++k1, ++k2) {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if (i != (_stackCount - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

        std::unique_ptr<Mesh> spMesh = std::make_unique<Mesh>();
        for (uint32_t idx : indices) {
            spMesh->vertices.emplace_back(vertices[idx]);
        }

        return spMesh;
    }
}  // namespace algorithm