//
// Created by paja on 1/10/22.
//

#ifndef MESH_H
#define MESH_H

#include <array>
#include <vector>
#include "Eigen/Dense"
#include "Material.h"

struct Ray;

struct Vertex {
    Eigen::Vector3f position;
    Eigen::Vector2f uv;
    Eigen::Vector3f normal;
    Eigen::Vector4f color;
};

struct Mesh {
    Mesh() {}
    std::vector<Vertex> vertices;
    bool bHasNormals = false;
    bool bHasUVs = false;
    bool bHasColor = false;
};
#endif