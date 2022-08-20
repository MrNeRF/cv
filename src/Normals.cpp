#include "Normals.h"
#include <Eigen/Dense>
#include <algorithm>
#include <iostream>
#include <numeric>
#include "Logger.h"

namespace algorithm {

    void ComputeSmoothNormals(Mesh* pMesh) {
        ASSERT(0)
        //        std::vector<Eigen::Vector3f> faceNormals;
        //        faceNormals.resize(m_rMesh.GetNumberOfFaces());
        //        std::vector<Eigen::Vector3f> vertexNormals;
        //        vertexNormals.resize(m_rMesh.GetNumberOfVertice());
        //
        //        int i = 0;
        //        for (const auto& faceID : faceIterator(m_rMesh)) {
        //            auto [vecA, vecB, vecC] = m_rMesh.GetFaceVertices(faceID);
        //            auto normal = ((vecB - vecA).cross(vecC - vecA));
        //            normal.normalize();
        //            faceNormals[faceID] = normal;
        //            /* std::cout << "Normal " << i << ": " << normal.x() << ", " << normal.y() << ", " << normal.z() << "\n"; */
        //            ++i;
        //        }
        //
        //        i = 0;
        //        for (const auto& vertexID : vertexIterator(m_rMesh)) {
        //            std::vector<Eigen::Vector3f> normals;
        //            for (const auto& faceID : oneRingFaceIterator(m_rMesh, vertexID)) {
        //                normals.push_back(faceNormals[faceID]);
        //            }
        //
        //            auto normal = std::accumulate(normals.begin(), normals.end(), Eigen::Vector3f(0.f, 0.f, 0.f));
        //            normal.normalize();
        //            vertexNormals[vertexID] = normal;
        //            /* std::cout << "Normal " << i << ": " << normal.x() << ", " << normal.y() << ", " << normal.z() << "\n"; */
        //            ++i;
        //        }
    }

    void ComputeNonSmoothNormals(Mesh* pMesh) {
        for (uint32_t idx = 0; idx < pMesh->vertices.size(); idx += 3) {
            const Eigen::Vector3f vecA = pMesh->vertices[idx].position;
            const Eigen::Vector3f vecB = pMesh->vertices[idx + 1].position;
            const Eigen::Vector3f vecC = pMesh->vertices[idx + 2].position;
            const Eigen::Vector3f normal = ((vecB - vecA).cross(vecC - vecA)).normalized();
            pMesh->vertices[idx].normal = normal;
            pMesh->vertices[idx + 1].normal = normal;
            pMesh->vertices[idx + 2].normal = normal;
        }
    }
}  // namespace algorithm