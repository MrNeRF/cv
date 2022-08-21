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
    void SetName(const std::string& rName) override { _name = rName; }
    const std::string& GetName(const std::string& rName) const override { return _name; };

    void ActivateShader(const IRenderable* pRenderObject) override;
    void SetMaterial(const Material& rMaterial);
    void SetLightSource(Light* pLight);
    const Light* GetLightSource() { return _pLight; };
    void SetCamera(std::shared_ptr<Camera> spCamera);

   private:
    std::string _name;
    Light* _pLight;
    std::shared_ptr<Camera> _spCamera;
};

#endif  // CV_PHONGSHADER_H
