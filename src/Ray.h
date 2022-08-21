#ifndef RAY_H
#define RAY_H

#include <Eigen/Dense>
#include <utility>

struct Ray {
    Ray() : Ray({0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, 0.f) {}
    Ray(Eigen::Vector3f origin, const Eigen::Vector3f direction, float t)
        : _origin{std::move(origin)}, _direction{direction}, _t{t} {
    }
    Eigen::Vector3f _origin;
    Eigen::Vector3f _direction;
    float _t = 0.f;
};

#endif