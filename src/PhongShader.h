//
// Created by paja on 6/23/22.
//

#ifndef CV_PHONGSHADER_H
#define CV_PHONGSHADER_H

#include <memory>
#include "Shader.h"

class Light;
class Material;
class Camera;
class IRenderable;

class PhongShader : public Shader {
   public:
    PhongShader();
    void ActivateShader(const IRenderable* pRenderObject) override;
    void SetMaterial(const Material& rMaterial);
    void SetLightSource(std::shared_ptr<Light> spLight);
    void SetCamera(std::shared_ptr<Camera> spCamera);

   private:
    std::unique_ptr<Material> _spMaterial;
    std::shared_ptr<Light> _spLight;
    std::shared_ptr<Camera> _spCamera;
};

#endif  // CV_PHONGSHADER_H
