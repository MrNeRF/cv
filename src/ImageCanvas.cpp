//
// Created by paja on 7/3/22.
//

#include "ImageCanvas.h"
#include <Mesh.h>
#include <Eigen/Dense>
#include "Logger.h"
#include "Shader.h"

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
    auto canvas = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajorBit>(6, 5);
    canvas(Triangle::FIRST_CORNER_A, COOR_X) = upperLeftCorner.x();
    canvas(Triangle::FIRST_CORNER_A, COOR_Y) = upperLeftCorner.y();
    canvas(Triangle::FIRST_CORNER_A, COOR_Z) = upperLeftCorner.z();
    canvas(Triangle::FIRST_CORNER_A, COOR_U) = upperLeftUV.x();
    canvas(Triangle::FIRST_CORNER_A, COOR_V) = upperLeftUV.y();

    canvas(Triangle::FIRST_CORNER_B, COOR_X) = upperRightCorner.x();
    canvas(Triangle::FIRST_CORNER_B, COOR_Y) = upperRightCorner.y();
    canvas(Triangle::FIRST_CORNER_B, COOR_Z) = upperRightCorner.z();
    canvas(Triangle::FIRST_CORNER_B, COOR_U) = upperRightUV.x();
    canvas(Triangle::FIRST_CORNER_B, COOR_V) = upperRightUV.y();

    canvas(Triangle::FIRST_CORNER_C, COOR_X) = lowerLeftCorner.x();
    canvas(Triangle::FIRST_CORNER_C, COOR_Y) = lowerLeftCorner.y();
    canvas(Triangle::FIRST_CORNER_C, COOR_Z) = lowerLeftCorner.z();
    canvas(Triangle::FIRST_CORNER_C, COOR_U) = lowerLeftUV.x();
    canvas(Triangle::FIRST_CORNER_C, COOR_V) = lowerLeftUV.y();

    canvas(Triangle::SECOND_CORNER_A, COOR_X) = upperRightCorner.x();
    canvas(Triangle::SECOND_CORNER_A, COOR_Y) = upperRightCorner.y();
    canvas(Triangle::SECOND_CORNER_A, COOR_Z) = upperRightCorner.z();
    canvas(Triangle::SECOND_CORNER_A, COOR_U) = upperRightUV.x();
    canvas(Triangle::SECOND_CORNER_A, COOR_V) = upperRightUV.y();

    canvas(Triangle::SECOND_CORNER_B, COOR_X) = lowerRightCorner.x();
    canvas(Triangle::SECOND_CORNER_B, COOR_Y) = lowerRightCorner.y();
    canvas(Triangle::SECOND_CORNER_B, COOR_Z) = lowerRightCorner.z();
    canvas(Triangle::SECOND_CORNER_B, COOR_U) = lowerRightUV.x();
    canvas(Triangle::SECOND_CORNER_B, COOR_V) = lowerRightUV.y();

    canvas(Triangle::SECOND_CORNER_C, COOR_X) = lowerLeftCorner.x();
    canvas(Triangle::SECOND_CORNER_C, COOR_Y) = lowerLeftCorner.y();
    canvas(Triangle::SECOND_CORNER_C, COOR_Z) = lowerLeftCorner.z();
    canvas(Triangle::SECOND_CORNER_C, COOR_U) = lowerLeftUV.x();
    canvas(Triangle::SECOND_CORNER_C, COOR_V) = lowerLeftUV.y();

    _spMesh = std::make_unique<Mesh>();
    _spMesh->faces = canvas;
    _spMesh->bHasUVs = true;
    _spMesh->bHasNormals = false;
}
void ImageCanvas::Render() {
    ASSERT(_spShader != nullptr);
    _spShader->ActivateShader(this);
}
void ImageCanvas::SetTexture(std::unique_ptr<Texture> spTexture) {
    ASSERT(spTexture != nullptr);
    _spTexture = std::move(spTexture);
}

void ImageCanvas::SetShader(std::unique_ptr<Shader> spShader) {
    ASSERT(spShader != nullptr);
    _spShader = std::move(spShader);
}
Mesh* ImageCanvas::GetMesh() {
    return _spMesh.get();
}

Texture* ImageCanvas::GetTexture() {
    return _spTexture.get();
}

const std::string& ImageCanvas::GetName() const {
    return _name;
}
