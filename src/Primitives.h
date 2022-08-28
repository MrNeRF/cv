//
// Created by paja on 8/20/22.
//

#ifndef CV_PRIMITIVES_H
#define CV_PRIMITIVES_H

#include <Eigen/Dense>

class Primitives {
   public:
    enum class Types {
        Sphere,
        Cuboid,
        Plane
    };

    explicit Primitives(Types type) : _type{type} {};
    virtual ~Primitives() = default;

    const Types _type;
};

class Cuboid : public Primitives {
   public:
    Cuboid();
    Cuboid(Eigen::Vector3f min, Eigen::Vector3f max);
    [[nodiscard]] Eigen::Vector3f GetCenter() const;
    [[nodiscard]] float GetWidth() const;
    [[nodiscard]] float GetHeight() const;
    [[nodiscard]] float GetDepth() const;
    [[nodiscard]] const Eigen::Vector3f& GetMin() const;
    [[nodiscard]] const Eigen::Vector3f& GetMax() const;
    Cuboid(const Cuboid& rCuboid);
    Cuboid& operator=(Cuboid rCuboid);

   private:
    Eigen::Vector3f _min;
    Eigen::Vector3f _max;
};

class Sphere : public Primitives {
   public:
    explicit Sphere();
    explicit Sphere(const Eigen::Vector3f& p0);
    explicit Sphere(Eigen::Vector3f p0, float radius);
    explicit Sphere(const Eigen::Vector3f& p0, const Eigen::Vector3f& p1);
    explicit Sphere(const Eigen::Vector3f& p0, const Eigen::Vector3f& p1, const Eigen::Vector3f& p2);
    explicit Sphere(const Eigen::Vector3f& p0, const Eigen::Vector3f& p1, const Eigen::Vector3f& p2, const Eigen::Vector3f& p3);
    Sphere(const Sphere& rSphere);
    Sphere& operator=(Sphere rSphere);

    [[nodiscard]] const Eigen::Vector3f& GetCenter() const { return _center; }
    [[nodiscard]] float GetRadius() const { return _radius; }

   private:
    Eigen::Vector3f _center;
    float _radius{};
};

class Plane : public Primitives {
   public:
    explicit Plane();
    explicit Plane(float width, float depth);
    float GetWidth() const { return _width; };
    float GetDepth() const { return _depth; };

   private:
    float _width;
    float _depth;
};
#endif  // CV_PRIMITIVES_H