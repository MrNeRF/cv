#ifndef BOUNDING_VOLUME_H
#define BOUNDING_VOLUME_H

#include <memory>
#include "Mesh.h"
#include "Primitives.h"

struct Ray;

namespace collision {
    class BoundingVolume {
       public:
        struct Intersection {
            Intersection(float tMin, Eigen::Vector3f hitPoint)
                : _tMin{tMin}, _hitPoint{std::move(hitPoint)} {
            }
            float _tMin;
            Eigen::Vector3f _hitPoint;
        };

       public:
        BoundingVolume() = default;
        BoundingVolume(const Primitives::Types& type, const Mesh& rMeshToBV);
        [[nodiscard]] const Primitives* GetBoundingVolume() const { return _spBoundingVolume.get(); };

        bool HasCollison(const Ray& rRay) const;
        bool HasCollison(const BoundingVolume& rBV) const;
        void Draw();

       private:
        void createBoundingSphere(const Mesh& rMeshToBound);
        void createBoundingCuboid(const Mesh& rMeshToBound);

       private:
        Primitives::Types _bvType;
        std::unique_ptr<Primitives> _spBoundingVolume;
    };

    bool MeshRayCollision(const Mesh& rMesh, const Ray& rRay, const Eigen::Vector3f& position, const Eigen::Matrix3f& model, const collision::BoundingVolume& rBV);
}  // namespace collision

#endif