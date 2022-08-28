//
// Created by paja on 6/23/22.
//

#ifndef CV_GRIDFLOORSHADER_H
#define CV_GRIDFLOORSHADER_H

#include <memory>
#include "Shader.h"

class Light;
class Material;
class Camera;
class IRenderable;

class GridFloorShader : public Shader {
   public:
    GridFloorShader();
    void SetName(const std::string& rName) override { _name = rName; }
    const std::string& GetName(const std::string& rName) const override { return _name; };

    void ActivateShader(const IRenderable* pRenderObject) override;
    void SetCamera(std::shared_ptr<Camera> spCamera);

   private:
    std::string _name;
    std::shared_ptr<Camera> _spCamera;
};

#endif
