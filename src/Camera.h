#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Dense>
#include <iostream>
#include "IObserver.h"
#include "Ray.h"

class Camera : public IObserver {
   public:
    Camera() = default;
    explicit Camera(Eigen::Vector3f& eye, Eigen::Vector3f& target, Eigen::Vector3f& up)
        : _eye{eye},
          _target{target},
          _up{up} {};
    void SetWindowSize(int width, int height) { _windowSize = Eigen::Vector2i(width, height); };
    const Eigen::Matrix4f& GetLookAt() const;
    void SetPerspectiveProjection(float fov, float aspectRatio, float zNearPlane, float zFarPlane);
    const Eigen::Matrix4f& GetPerspectiveProjection() const;
    void UpdateOrientation(const Eigen::AngleAxisf& angleAxis);
    const Eigen::Vector3f& GetCameraPosition() const { return _eye; }
    const Ray& GetCameraRay() const { return _cameraRay; }
    void UpdateCameraPosition(Eigen::Vector2d deltaCursorPos, double delta_time);
    float GetFarPlaneValue() const { return _zFar; }
    float GetNearPlaneValue() const { return _zNear; }

    // Observer
    void Update(const InputEvent::IEvent& rEvent) override;

   private:
    float _fov = 45.f;
    float _zNear = .01f;
    float _zFar = 100.f;
    float _aspectRatio = 0.75f;
    Eigen::Vector3f _eye = Eigen::Vector3f(0.f, 1.f, 8.f);
    Eigen::Vector3f _target = Eigen::Vector3f(0.f, 0.f, 0.f);
    Eigen::Vector3f _up = Eigen::Vector3f(0.f, 1.f, 0.f);
    Eigen::Matrix4f _frustum;
    mutable Eigen::Matrix4f _view;
    Eigen::Vector2i _windowSize;
    Ray _cameraRay;
};

#endif
