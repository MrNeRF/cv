//
// Created by paja on 6/22/22.
//

#ifndef CV_MATERIAL_H
#define CV_MATERIAL_H

#include <Eigen/Dense>

class Material {
   public:
    Material() = default;
    Material(const Eigen::Vector3f& ambient,
             const Eigen::Vector3f& diffuse,
             const Eigen::Vector3f& specular,
             const float shininess) : _ambient{ambient},
                                      _diffuse{diffuse},
                                      _specular{specular},
                                      _shininess{shininess} {}

    virtual ~Material() = default;
    const Eigen::Vector3f& GetAmbient() { return _ambient; }
    const Eigen::Vector3f& GetDiffuse() { return _diffuse; }
    const Eigen::Vector3f& GetSpecular() { return _specular; }
    float GetShininess() { return _shininess; }

   private:
    Eigen::Vector3f _ambient{.5f, .5f, .5f};
    Eigen::Vector3f _diffuse{.2f, .2f, .2f};
    Eigen::Vector3f _specular{.2f, .2f, .2f};
    float _shininess = 0.8;
};

#endif  // CV_MATERIAL_H
