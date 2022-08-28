#include "Camera.h"
#include <Logger.h>
#include <algorithm>
#include <cmath>
#include <numbers>

static Eigen::Matrix4f ComputeFrustum(float left, float right, float bottom, float top, float near, float far);

template <typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

static float maxRotateRadPerSec = std::numbers::pi_v<float>;

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

void Camera::UpdateCameraPosition(Eigen::Vector2d deltaCursorPos, double delta_time) {
    static const float sensitivity = 3.f;

    const float maxRotRadPerTimeInstance = delta_time * maxRotateRadPerSec;
    if (deltaCursorPos.y() != 0.) {
        const float maxRotRadPerTimeInstanceSensitive = maxRotRadPerTimeInstance * 1.5f;
        const float len = std::sqrt(std::pow(_eye.x(), 2.f) + std::pow(_eye.z(), 2.f));
        const float sign = sgn(-deltaCursorPos.y());
        const float maxRadY = 1.55f;
        const float rad_y = -1.f * std::atan2(_eye.y(), len);
        const float rotMax = (std::abs(rad_y + sign * maxRotRadPerTimeInstanceSensitive) > maxRadY ? (maxRadY - std::abs(rad_y)) : maxRotRadPerTimeInstanceSensitive);
        const Eigen::Vector3f camDir = (_eye - _target).normalized();
        const Eigen::Vector3f camRight = (_up.cross(camDir)).normalized();

        const Eigen::Affine3f rot_pitch(Eigen::AngleAxis<float>(sign * rotMax, camRight));
        _eye = rot_pitch * _eye;
    }

    if (deltaCursorPos.x() != 0.) {
        const float sign = sgn(-deltaCursorPos.x());
        const Eigen::Affine3f rot_yaw(Eigen::AngleAxis<float>(sensitivity * sign * maxRotRadPerTimeInstance, Eigen::Vector3f::UnitY()));
        _eye = rot_yaw * _eye;
    }
}
void Camera::Update(const InputEvent::IEvent& rEvent) {
    switch (rEvent.eventType) {
    case InputEvent::InputEventType::MouseButton: {
        const auto& event = dynamic_cast<const InputEvent::MouseButton&>(rEvent);
        const auto NDC = Eigen::Vector2f(2.f * event.cursorPosition(0) / _windowSize(0), 2.f * event.cursorPosition(1) / _windowSize(1));
        const auto pixelScreen = Eigen::Vector2f(NDC[0] - 1, 1 - NDC[1]);
        const Eigen::Vector4f ray_clipped = Eigen::Vector4f(pixelScreen[0], pixelScreen[1], -1, 1);
        Eigen::Vector4f ray_eye = _frustum.inverse() * ray_clipped;
        ray_eye = Eigen::Vector4f(ray_eye.x(), ray_eye.y(), -1.f, 0.f);

        Eigen::Vector4f ray_world4f = (GetLookAt().inverse() * ray_eye);
        Eigen::Vector3f ray_world = Eigen::Vector3f(ray_world4f.x(), ray_world4f.y(), ray_world4f.z()).normalized();
        _cameraRay = Ray(_eye, ray_world, 0.f);
    } break;
    case InputEvent::InputEventType::MouseWheel: {
        const auto& event = dynamic_cast<const InputEvent::MouseWheel&>(rEvent);
        _fov -= static_cast<float>(event.yOffeset) * .05f;
        std::cout << _fov << "\n";
        SetPerspectiveProjection(_fov, _aspectRatio, _zNear, _zFar);
    } break;
    case InputEvent::InputEventType::WindowResize: {
        const auto& event = dynamic_cast<const InputEvent::WindowResize&>(rEvent);
        _windowSize = Eigen::Vector2i(event.width, event.height);
    } break;
    default:
        break;
    }
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
