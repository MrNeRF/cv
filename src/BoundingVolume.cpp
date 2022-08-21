#include "BoundingVolume.h"
#include <deque>
#include <limits>
#include "Logger.h"
#include "Primitives.h"
#include "Ray.h"

namespace collision {
    // Welzl Miniball Algorithm
    static Sphere recursiveMB(std::deque<Eigen::Vector3f>& points, std::vector<Eigen::Vector3f> bound) {
        Sphere mb;
        if (points.empty() || bound.size() == 4) {
            switch (bound.size()) {
            case 0:
                mb = Sphere();
                break;
            case 1:
                mb = Sphere(bound[0]);
                break;
            case 2:
                mb = Sphere(bound[0], bound[1]);
                break;
            case 3:
                mb = Sphere(bound[0], bound[1], bound[2]);
                break;
            case 4:
                mb = Sphere(bound[0], bound[1], bound[2], bound[3]);
                break;
            }
        } else {
            Eigen::Vector3f p = points.back();
            points.pop_back();
            mb = recursiveMB(points, bound);
            const Eigen::Vector3f sc = mb.GetCenter();
            const float r2 = mb.GetRadius() * mb.GetRadius();
            if ((sc - p).squaredNorm() > r2) {
                bound.push_back(p);
                mb = recursiveMB(points, bound);
                // move to front
                points.push_back(p);
            }
        }

        return mb;
    }

    static bool HasCollision(const Sphere& rSphere, const Ray& rRay) {
        ASSERT(std::fabs(rRay._direction.norm() - 1.f) <= std::numeric_limits<float>::epsilon());
        const Eigen::Vector3f m = rRay._origin - rSphere.GetCenter();
        const float b = m.dot(rRay._direction);
        const float c = m.dot(m) - rSphere.GetRadius() * rSphere.GetRadius();
        if (c > 0.f && b > 0.f) {
            return false;
        } else {
            const float discriminant = b * b - c;
            if (discriminant < 0.f) {
                return false;
            } else {

                const float tmpLength = -b - std::sqrt(discriminant);
                const float t = tmpLength < 0.f ? 0.f : tmpLength;
                // Hitpoint: rRay.m_origin + t * rRay.m_direction
                if (t < 0.f) {
                    return false;
                } else {
                    return true;
                }
            }
        }
    }

    static bool HasCollision(const Cuboid& rCuboid, const Ray& rRay) {
        float tMin = 0.f;
        float tMax = std::numeric_limits<float>::max();

        Eigen::Vector3f min = rCuboid.GetMin();
        Eigen::Vector3f max = rCuboid.GetMax();

        for (uint32_t i = 0; i < 3; ++i) {
            if (std::abs(rRay._direction[i]) < std::numeric_limits<float>::epsilon()) {
                if (rRay._origin[i] < min[i] || rRay._origin[i] > max[i])
                    return false;
            } else {
                float r = 1.f / rRay._direction[i];
                float t1 = (min[i] - rRay._origin[i]) * r;
                float t2 = (max[i] - rRay._origin[i]) * r;
                if (t1 > t2) {
                    std::swap(t1, t2);
                }

                tMin = std::max(tMin, t1);
                tMax = std::min(tMax, t2);
                if (tMin > tMax) {
                    return false;
                }
            }
        }
        //        Intersection(tMin, rRay._origin + tMin * rRay._direction);
        return true;
    }

    static bool HasCollision(const Cuboid& rCuboid, const Cuboid& rOtherCuboid) {
        if (rCuboid.GetMax()[0] < rOtherCuboid.GetMin()[0] || rCuboid.GetMin()[0] > rOtherCuboid.GetMax()[0])
            return false;
        if (rCuboid.GetMax()[1] < rOtherCuboid.GetMin()[1] || rCuboid.GetMin()[1] > rOtherCuboid.GetMax()[1])
            return false;
        if (rCuboid.GetMax()[2] < rOtherCuboid.GetMin()[2] || rCuboid.GetMin()[2] > rOtherCuboid.GetMax()[2])
            return false;
        return true;
    }

    static bool HasCollision(const Cuboid& rOtherCuboid, const Sphere& rSphere) {
        ASSERT(0)
        // TODO: implement
        return false;
    }

    static bool HasCollision(const Sphere& rSphere, const Cuboid& rOtherCuboid) {
        return HasCollision(rOtherCuboid, rSphere);
    }

    static bool HasCollision(const Sphere& rSphere, const Sphere& rOtherSphere) {
        const float dist = (rSphere.GetCenter() - rOtherSphere.GetCenter()).norm();
        if (dist < (rSphere.GetRadius() + rOtherSphere.GetRadius())) {
            return true;
        } else {
            return false;
        }
    }

    BoundingVolume::BoundingVolume(const Primitives::Types& type, const Mesh& rMeshToBV)
        : _bvType{type} {

        switch (_bvType) {
        case Primitives::Types::Sphere:
            createBoundingSphere(rMeshToBV);
            break;
        case Primitives::Types::Cuboid:
            createBoundingCuboid(rMeshToBV);
            break;
        }
    }

    void BoundingVolume::createBoundingCuboid(const Mesh& rMeshToBound) {
        Eigen::Vector3f max = Eigen::Vector3f(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
        Eigen::Vector3f min = Eigen::Vector3f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

        for (const auto& rVertex : rMeshToBound.vertices) {
            const auto& v = rVertex.position;
            max[0] = std::max(max[0], v[0]);
            max[1] = std::max(max[1], v[1]);
            max[2] = std::max(max[2], v[2]);

            min[0] = std::min(min[0], v[0]);
            min[1] = std::min(min[1], v[1]);
            min[2] = std::min(min[2], v[2]);
        }

        _spBoundingVolume = std::make_unique<Cuboid>(min, max);
    }

    void BoundingVolume::createBoundingSphere(const Mesh& rMeshToBound) {
        std::deque<Eigen::Vector3f> input;
        for (const auto& rVertex : rMeshToBound.vertices) {
            input.push_back(rVertex.position);
        }
        std::vector<Eigen::Vector3f> bound;

        _spBoundingVolume = std::make_unique<Sphere>(recursiveMB(input, bound));
    }

    bool BoundingVolume::HasCollison(const Ray& rRay) const {
        bool bHasCollison = false;
        switch (_bvType) {
        case Primitives::Types::Sphere:
            bHasCollison = HasCollision(dynamic_cast<const Sphere&>(*_spBoundingVolume), rRay);
            break;
        case Primitives::Types::Cuboid:
            bHasCollison = HasCollision(dynamic_cast<const Cuboid&>(*_spBoundingVolume), rRay);
            break;
        }

        return bHasCollison;
    }

    bool BoundingVolume::HasCollison(const BoundingVolume& rBV) const {
        bool bHasCollison = false;
        switch (_bvType) {
        case Primitives::Types::Sphere: {
            switch (rBV._bvType) {
            case Primitives::Types::Sphere:
                bHasCollison = HasCollision(dynamic_cast<const Sphere&>(*_spBoundingVolume), dynamic_cast<const Sphere&>(*rBV.GetBoundingVolume()));
                break;
            case Primitives::Types::Cuboid:
                bHasCollison = HasCollision(dynamic_cast<const Sphere&>(*_spBoundingVolume), dynamic_cast<const Cuboid&>(*rBV.GetBoundingVolume()));
                break;
            }
        } break;
        case Primitives::Types::Cuboid: {
            switch (rBV._bvType) {
            case Primitives::Types::Sphere:
                bHasCollison = HasCollision(dynamic_cast<const Cuboid&>(*_spBoundingVolume), dynamic_cast<const Sphere&>(*rBV.GetBoundingVolume()));
                break;
            case Primitives::Types::Cuboid:
                bHasCollison = HasCollision(dynamic_cast<const Cuboid&>(*_spBoundingVolume), dynamic_cast<const Cuboid&>(*rBV.GetBoundingVolume()));
                break;
            }
        } break;
        }
        return bHasCollison;
    }

    void BoundingVolume::Draw() {
    }

    bool MeshRayCollision(const Mesh& rMesh, const Ray& rRay, const Eigen::Vector3f& position, const Eigen::Matrix3f& model, const collision::BoundingVolume& rBV) {
        if (!rBV.HasCollison(rRay)) {
            return false;
        }

        for (size_t idx = 0; idx < rMesh.vertices.size(); idx += 3) {
            Eigen::Vector3f vec0 = rMesh.vertices[idx].position;
            Eigen::Vector3f vec1 = rMesh.vertices[idx + 1].position;
            Eigen::Vector3f vec2 = rMesh.vertices[idx + 2].position;
            vec0 = model * vec0 + position;
            vec1 = model * vec1 + position;
            vec2 = model * vec2 + position;
            Eigen::Vector3f vecA = vec1 - vec0;
            Eigen::Vector3f vecB = vec2 - vec0;
            Eigen::Vector3f pVec = rRay._direction.cross(vecB);
            float det = vecA.dot(pVec);
            if (std::abs(det) < std::numeric_limits<float>::epsilon()) {
                continue;
            }
            const float invDet = 1.f / det;
            Eigen::Vector3f tvec = rRay._origin - vec0;
            const float u = tvec.dot(pVec) * invDet;
            if (u < 0.f || u > 1.f) {
                continue;
            }

            const Eigen::Vector3f qvec = tvec.cross(vecA);
            const float v = rRay._direction.dot(qvec) * invDet;
            if ((v < 0) || u + v > 1.f) {
                continue;
            }
            std::cout << "Object hit" << std::endl;
            return true;
        }
        return false;
    }

}  // namespace collision