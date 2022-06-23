#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Dense>

class Camera {
   public:
    Camera() = default;
    explicit Camera(Eigen::Vector3f& eye, Eigen::Vector3f& target, Eigen::Vector3f& up)
        : m_eye{eye},
          m_target{target},
          m_up{up} {};
    const Eigen::Matrix4f& GetLookAt() const;
    void SetPerspectiveProjection(float fov, float aspectRatio, float zNearPlane, float zFarPlane);
    const Eigen::Matrix4f& GetPerspectiveProjection() const;
    void UpdateOrientation(const Eigen::AngleAxisf& angleAxis);
    const Eigen::Vector3f& GetCameraPosition() const { return m_eye; }

    // Observer
   private:
    float m_fov = 45.f;
    float m_zNear = 1.f;
    float m_zFar = 50.f;
    float m_aspectRatio = 0.75f;
    Eigen::Vector3f m_eye = Eigen::Vector3f(0.f, 0.f, 2.f);
    Eigen::Vector3f m_target = Eigen::Vector3f(0.f, 0.f, 0.f);
    Eigen::Vector3f m_up = Eigen::Vector3f(0.f, 1.f, 0.f);
    Eigen::Matrix4f m_frustum;
    mutable Eigen::Matrix4f m_view;
};

#endif
