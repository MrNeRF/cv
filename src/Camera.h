#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Dense>
#include <iostream>

class Camera {
   public:
    Camera() = default;
    explicit Camera(Eigen::Vector3f& eye, Eigen::Vector3f& target, Eigen::Vector3f& up)
        : _eye{eye},
          _target{target},
          _up{up} {};
    const Eigen::Matrix4f& GetLookAt() const;
    void SetPerspectiveProjection(float fov, float aspectRatio, float zNearPlane, float zFarPlane);
    const Eigen::Matrix4f& GetPerspectiveProjection() const;
    void UpdateOrientation(const Eigen::AngleAxisf& angleAxis);
    const Eigen::Vector3f& GetCameraPosition() const { return _eye; }
    void UpdateCameraPosition(Eigen::Vector3f pos) { _eye += pos; }

    // Observer
   private:
    float _fov = 45.f;
    float _zNear = 1.f;
    float _zFar = 50.f;
    float _aspectRatio = 0.75f;
    Eigen::Vector3f _eye = Eigen::Vector3f(0.f, 0.f, 2.f);
    Eigen::Vector3f _target = Eigen::Vector3f(0.f, 0.f, 0.f);
    Eigen::Vector3f _up = Eigen::Vector3f(0.f, 1.f, 0.f);
    Eigen::Matrix4f _frustum;
    mutable Eigen::Matrix4f _view;
};

#endif
