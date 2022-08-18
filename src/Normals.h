#ifndef ALGO_VERTEX_NORMALS
#define ALGO_VERTEX_NORMALS

#include "Mesh.h"

namespace algorithm {
    static void ComputeSmoothNormals(Mesh* pMesh);
    static void ComputeNonSmoothNormals(Mesh* pMesh);
}  // namespace algorithm

#endif
