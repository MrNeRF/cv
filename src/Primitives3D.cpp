#include "Primitives3D.h"

std::unique_ptr<Mesh> Primitives3D::CreateCube(const int32_t width, const int32_t height, const int32_t depth) {
    std::vector<Eigen::Vector3f> vertices;
    vertices.reserve(8);
    std::vector<uint32_t> indices;
    indices.reserve(36);

    const float w = width * 0.5f;
    const float h = height * 0.5f;
    const float d = depth * 0.5f;

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