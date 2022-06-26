#include "Camera.h"
#include <Logger.h>
#include <cmath>

static Eigen::Matrix4f ComputeFrustum(float left, float right, float bottom, float top, float near, float far);

const Eigen::Matrix4f& Camera::GetLookAt() const {
    // Right Hand Coordinate System
    Eigen::Vector3f camDir = (_eye - _target).normalized();
    Eigen::Vector3f camRight = (_up.cross(camDir)).normalized();
    Eigen::Vector3f camUp = camDir.cross(camRight);

    _view.block(0, 0, 1, 3) = camRight.transpose();
    _view.block(1, 0, 1, 3) = camUp.transpose();
    _view.block(2, 0, 1, 3) = camDir.transpose();
    _view(0, 3) = -camRight.dot(_eye);
    _view(1, 3) = -camUp.dot(_eye);
    _view(2, 3) = -camDir.dot(_eye);
    _view(3, 3) = 1.f;

    return _view;
}

void Camera::SetPerspectiveProjection(float fov, float aspectRatio, float zNearPlane, float zFarPlane) {
    _zFar = zFarPlane;
    _zNear = zNearPlane;
    _fov = fov;
    _aspectRatio = aspectRatio;

    float height = _zNear * tanf(_fov * .5f);
    float width = height * _aspectRatio;

    _frustum = ComputeFrustum(-width, width, -height, height, _zNear, _zFar);
}

const Eigen::Matrix4f& Camera::GetPerspectiveProjection() const {
    ASSERT(!_frustum.isZero())
    return _frustum;
}

void Camera::UpdateOrientation(const Eigen::AngleAxisf& angleAxis) {
    Eigen::Quaternionf rotationQuat = Eigen::Quaternionf(angleAxis);
    Eigen::Quaternionf tmp = rotationQuat * Eigen::Quaternionf(_eye.x(), _eye.y(), _eye.z(), 0.f) * rotationQuat.conjugate();
    _eye.x() = tmp.x();
    _eye.y() = tmp.y();
    _eye.z() = tmp.z();
}

Eigen::Matrix4f ComputeFrustum(float left, float right, float bottom, float top, float near, float far) {
    Eigen::Matrix4f projection = Eigen::Matrix4f::Zero(4, 4);

    projection(0, 0) = 2.f * near / (right - left);
    projection(0, 2) = (right + left) / (right - left);
    projection(1, 1) = 2.f * near / (top - bottom);
    projection(1, 2) = (top + bottom) / (top - bottom);
    projection(2, 2) = -(far + near) / (far - near);
    projection(2, 3) = -(2.f * far * near) / (far - near);
    projection(3, 2) = -1.f;

    return projection;
}
