//
// Created by paja on 6/23/22.
//

#ifndef CV_COLORSHADER_H
#define CV_COLORSHADER_H

#include <memory>
#include "Shader.h"

class Light;
class Material;
class Camera;
class IRenderable;

class ColorShader : public Shader {
   public:
    explicit ColorShader(const Eigen::Vector4f& rColor);
    void SetName(const std::string& rName) override { _name = rName; }
    const std::string& GetName(const std::string& rName) const override { return _name; };

    void ActivateShader(const IRenderable* pRenderObject) override;
    void SetCamera(std::shared_ptr<Camera> spCamera);
    void SetColor(const Eigen::Vector4f& rColor);

   private:
    std::string _name;
    Eigen::Vector4f _color;
    std::shared_ptr<Camera> _spCamera;
};

#endif  // CV_COLORSHADER_H
