//
// Created by paja on 7/3/22.
//

#ifndef CV_CANVASSHADER_H
#define CV_CANVASSHADER_H

#include "Shader.h"
class IRenderable;
class Camera;

class CanvasShader : public Shader {
   public:
    CanvasShader();
    void ActivateShader(const IRenderable* pRenderObject) override;
    void SetCamera(std::shared_ptr<Camera> spCamera);

   private:
    std::shared_ptr<Camera> _spCamera;
};

#endif  // CV_CANVASSHADER_H
