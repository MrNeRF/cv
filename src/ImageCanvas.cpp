//
// Created by paja on 7/3/22.
//

#include "ImageCanvas.h"
#include <Eigen/Dense>
#include "Logger.h"
#include "Shader.h"
#include "RenderUnit.h"

enum Position {
    POS_UPPER_LEFT = 0,
    POS_UPPER_RIGHT = 1,
    POS_LOWER_LEFT = 2,
    POS_LOWER_RIGHT = 3,
};

enum COORDINATE {
    COOR_X = 0,
    COOR_Y = 1,
    COOR_Z = 2,
    COOR_U = 3,
    COOR_V = 4
};

enum Triangle {
    FIRST_CORNER_A = 0,
    FIRST_CORNER_B = 1,
    FIRST_CORNER_C = 2,
    SECOND_CORNER_A = 3,
    SECOND_CORNER_B = 4,
    SECOND_CORNER_C = 5,
};
Eigen::Vector3f upperLeftCorner = {-0.5f, .5f, 0.f};
Eigen::Vector3f upperRightCorner = {.5f, .5f, 0.f};
Eigen::Vector3f lowerLeftCorner = {-.5f, -.5f, 0.f};
Eigen::Vector3f lowerRightCorner = {.5f, -.5f, 0.f};
// indices UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT,  UPPER_RIGHT,  LOWER_RIGHT, LOWER_LEFT

Eigen::Vector2f upperLeftUV = {0.f, 1.f};
Eigen::Vector2f upperRightUV = {1.f, 1.f};
Eigen::Vector2f lowerLeftUV = {0.f, 0.f};
Eigen::Vector2f lowerRightUV = {1.f, 0.f};
// indices

// Eigen::Vector3f upperLeftCorner = {-1.f, -1.f, 0.f};
// spMesh->faces = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajorBit>(indices.size() / 3, 3 + 2 + 3);
// auto& faces = spMesh->faces;
// for (size_t i = 0, j = 0; i < indices.size(); i += 3, ++j) {
// const Eigen::Vector3f& v = vertices[indices[i]];
// const Eigen::Vector2f& uv = uvs[indices[i + 1]];
// const Eigen::Vector3f& n = normals[indices[i + 2]];
// faces(j, 0) = v.x();
// faces(j, 1) = v.y();
// faces(j, 2) = v.z();
// faces(j, 3) = uv.x();
// faces(j, 4) = uv.y();

ImageCanvas::ImageCanvas() {
//
//    _spMesh = std::make_unique<Mesh>();
//    _spMesh->vertices.reserve(6);
//    _spMesh->bHasUVs = true;
//    _spMesh->bHasNormals = false;
//
//    Vertex v0, v1, v2, v3, v4, v5;
//
//    v0.position = upperLeftCorner;
//    v0.uv = upperLeftUV;
//
//    v1.position = upperRightCorner;
//    v1.uv = upperRightUV;
//
//    v2.position = lowerLeftCorner;
//    v2.uv = lowerLeftUV;
//
//    v3.position = upperRightCorner;
//    v3.uv = upperRightUV;
//
//    v4.position = lowerRightCorner;
//    v4.uv = lowerRightUV;
//
//    v5.position = lowerLeftCorner;
//    v5.uv = lowerLeftUV;
//
//    _spMesh->vertices.push_back(v0);
//    _spMesh->vertices.push_back(v1);
//    _spMesh->vertices.push_back(v2);
//    _spMesh->vertices.push_back(v3);
//    _spMesh->vertices.push_back(v4);
//    _spMesh->vertices.push_back(v5);
}

void ImageCanvas::Render() {
//    ASSERT(_spShader != nullptr);
//    _spShader->ActivateShader(this);
}

const Shader* ImageCanvas::AddShader(std::unique_ptr<Shader> spShader) {
//    ASSERT(spShader != nullptr);
    _shader.push_back(std::move(spShader));
    return _shader.back().get();
}

const std::string& ImageCanvas::GetName() const {
    return _name;
}
