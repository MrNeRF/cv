//
// Created by paja on 8/20/22.
//

#include "Primitives.h"
#include <utility>
#include "Logger.h"
#include "Macros.h"

Cuboid::Cuboid() : Cuboid({0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}) {
}

Cuboid::Cuboid(Eigen::Vector3f min, Eigen::Vector3f max) : Primitives(Primitives::Types::Cuboid),
                                                           _min{std::move(min)},
                                                           _max{std::move(max)}

{
}

Cuboid::Cuboid(const Cuboid& rCuboid) : Primitives(rCuboid._type) {
    _min = rCuboid._min;
    _max = rCuboid._max;
}

Cuboid& Cuboid::operator=(Cuboid rCuboid) {
    std::swap(_min, rCuboid._min);
    std::swap(_max, rCuboid._max);
    return *this;
}

Eigen::Vector3f Cuboid::GetCenter() const {
    return (_min + _max) * 0.5f;
}

float Cuboid::GetWidth() const {
    return _max.x() - _min.x();
}

float Cuboid::GetHeight() const {
    return _max.y() - _min.y();
}

float Cuboid::GetDepth() const {
    return _max.z() - _min.z();
}

const Eigen::Vector3f& Cuboid::GetMin() const {
    return _min;
}

const Eigen::Vector3f& Cuboid::GetMax() const {
    return _max;
}

Sphere::Sphere() : Sphere(Eigen::Vector3f(0.f, 0.f, 0.f)) {
}

Sphere::Sphere(const Eigen::Vector3f& p0) : Sphere(p0, 1.f) {
}

Sphere::Sphere(Eigen::Vector3f p0, float radius) : Primitives(Primitives::Types::Sphere),
                                                   _center{std::move(p0)},
                                                   _radius(radius) {
}

Sphere::Sphere(const Eigen::Vector3f& p0, const Eigen::Vector3f& p1) : Sphere((p0 + p1) * 0.5f, std::abs((p1 - p0).norm() * 0.5f)) {
}

Sphere::Sphere(const Eigen::Vector3f& p0, const Eigen::Vector3f& p1, const Eigen::Vector3f& p2) : Sphere() {
    ASSERT(p0 != p1)
    ASSERT(p0 != p2)
    ASSERT(p1 != p2)
    Eigen::Vector3f a = p0 - p2;
    Eigen::Vector3f b = p1 - p2;

    Eigen::Vector3f aXb = a.cross(b);
    _radius = a.norm() * b.norm() * (a - b).norm() / (2.f * aXb.norm());
    _center = (a.squaredNorm() * b - b.squaredNorm() * a).cross(aXb) / (2.f * aXb.squaredNorm()) + p2;
}

Sphere::Sphere(const Eigen::Vector3f& p0, const Eigen::Vector3f& p1, const Eigen::Vector3f& p2, const Eigen::Vector3f& p3) : Sphere() {
    Eigen::Matrix<float, 5, 5, Eigen::RowMajor> C = Eigen::Matrix<float, 5, 5, Eigen::RowMajor>::Ones();
    C(0, 0) = 0.f;
    C(1, 1) = 0.f;
    C(2, 2) = 0.f;
    C(3, 3) = 0.f;
    C(4, 4) = 0.f;
    const float d12 = (p1 - p0).squaredNorm();
    const float d13 = (p2 - p0).squaredNorm();
    const float d14 = (p3 - p0).squaredNorm();
    const float d23 = (p2 - p1).squaredNorm();
    const float d24 = (p3 - p1).squaredNorm();
    const float d34 = (p3 - p2).squaredNorm();

    C(1, 2) = d12;
    C(1, 3) = d13;
    C(1, 4) = d14;

    C(2, 1) = d12;
    C(2, 3) = d23;
    C(2, 4) = d24;

    C(3, 1) = d13;
    C(3, 2) = d23;
    C(3, 4) = d34;

    C(4, 1) = d14;
    C(4, 2) = d24;
    C(4, 3) = d34;

    Eigen::Matrix<float, 5, 5, Eigen::RowMajor> M = -2 * C.inverse();
    Eigen::VectorXf m = M.row(0).transpose();
    if (math::is_nan(m)) {
        Sphere sphere(p0, p1, p2);
        _radius = sphere._radius;
        _center = sphere._center;
    } else {
        _radius = 0.5f * std::sqrt(m(0));
        _center = 1.f / (m(1) + m(2) + m(3) + m(4)) * (m(1) * p0 + m(2) * p1 + m(3) * p2 + m(4) * p3);
    }
}

Sphere::Sphere(const Sphere& rSphere) : Primitives(rSphere._type) {
    _radius = rSphere._radius;
    _center = rSphere._center;
}
Sphere& Sphere::operator=(Sphere rSphere) {
    std::swap(_radius, rSphere._radius);
    std::swap(_center, rSphere._center);
    return *this;
}