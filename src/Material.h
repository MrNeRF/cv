//
// Created by paja on 6/22/22.
//

#ifndef CV_MATERIAL_H
#define CV_MATERIAL_H

#include <Eigen/Dense>
#include "Texture.h"
#include <memory>

struct Material {
    std::string materialName;
    Eigen::Vector3f ambient{.5f, .5f, .5f};
    Eigen::Vector3f diffuse{.2f, .2f, .2f};
    Eigen::Vector3f specular{.2f, .2f, .2f};
    float specularExponent = 0.8;
    std::unique_ptr<Texture> spTexure;
};

#endif  // CV_MATERIAL_H
