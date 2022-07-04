//
// Created by paja on 1/10/22.
//

#ifndef MESH_H
#define MESH_H

#include <array>
#include <vector>
#include "Eigen/Dense"

struct Mesh {
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajorBit> faces;
    bool bHasVertices = true;
    bool bHasUVs = false;
    bool bHasNormals = false;
};

#endif  // LEARNOPENGL_MESH_H
