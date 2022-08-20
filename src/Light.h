#ifndef CV_LIGHT_H
#define CV_LIGHT_H

#include <Eigen/Dense>

class Light {
   public:
    Light() = default;
    Light(const Eigen::Vector3f& ambient,
          const Eigen::Vector3f& diffuse,
          const Eigen::Vector3f& specular,
          const Eigen::Vector4f& position) : _ambient{ambient},
                                             _diffuse{diffuse},
                                             _specular{specular},
                                             _position{position} {}

    virtual ~Light() = default;
    const Eigen::Vector3f& GetAmbient() const { return _ambient; }
    const Eigen::Vector3f& GetDiffuse() const { return _diffuse; }
    const Eigen::Vector3f& GetSpecular() const { return _specular; }
    const Eigen::Vector4f& GetPostion() const { return _position; }

   private:
    Eigen::Vector3f _ambient{.5f, .5f, .5f};
    Eigen::Vector3f _diffuse{.2f, .2f, .2f};
    Eigen::Vector3f _specular{.2f, .2f, .2f};
    Eigen::Vector4f _position{1.f, 2.f, 6.f, 1.f};
};
#endif  // CV_LIGHT_H
