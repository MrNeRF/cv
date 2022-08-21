#ifndef RAY_H
#define RAY_H

#include <Eigen/Dense>

struct Ray {
    Ray(const Eigen::Vector3f& origin, const Eigen::Vector3f direction, float t)
        : _origin{origin}, _direction{direction}, _t{t} {
    }
    Eigen::Vector3f _origin;
    Eigen::Vector3f _direction;
    float _t = 0.f;
};

#endif