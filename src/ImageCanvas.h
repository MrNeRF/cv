//
// Created by paja on 7/3/22.
//

#ifndef CV_IMAGECANVAS_H
#define CV_IMAGECANVAS_H

#include "IRenderable.h"
#include <memory>

struct Mesh;

class ImageCanvas : public IRenderable {
   public:
    ImageCanvas();
    void SetShader(std::unique_ptr<Shader> spShader) override;
    void SetTexture(std::unique_ptr<Texture> spTexture) override;
    void Render() override;
    Mesh* GetMesh() override;
    Texture* GetTexture() override;
    const std::string& GetName() const override;
   private:
    std::unique_ptr<Mesh> _spMesh;
};

#endif  // CV_IMAGECANVAS_H
