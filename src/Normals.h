#ifndef ALGO_VERTEX_NORMALS
#define ALGO_VERTEX_NORMALS

#include "Mesh.h"

namespace algorithm {
    [[maybe_unused]] static void ComputeSmoothNormals(Mesh* pMesh);
    [[maybe_unused]] static void ComputeNonSmoothNormals(Mesh* pMesh);
}  // namespace algorithm

#endif
