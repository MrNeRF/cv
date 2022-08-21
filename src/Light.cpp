#include "Light.h"

Light::Light(const std::string& rName) : Light(rName,
                                               {.5f, .5f, .5f},
                                               {.2f, .2f, .2f},
                                               {.2f, .2f, .2f},
                                               {1.f, 2.f, 6.f, 1.f}) {}

Light::Light(std::string name,
             Eigen::Vector3f ambient,
             Eigen::Vector3f diffuse,
             Eigen::Vector3f specular,
             Eigen::Vector4f position) : _name{std::move(name)},
                                         _ambient{std::move(ambient)},
                                         _diffuse{std::move(diffuse)},
                                         _specular{std::move(specular)},
                                         _position{std::move(position)} {}
void Light::Render() {
}
Shader* Light::AddShader(std::unique_ptr<Shader> spShader) {
   _spShader = std::move(spShader);
   return _spShader.get();
}
