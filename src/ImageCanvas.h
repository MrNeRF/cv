//
// Created by paja on 7/3/22.
//

#ifndef CV_IMAGECANVAS_H
#define CV_IMAGECANVAS_H

#include <memory>
#include "IRenderable.h"

struct Mesh;
class Shader;
struct Texture;
struct RenderUnit;

class ImageCanvas : public IRenderable {
   public:
    ImageCanvas();

    virtual const Shader* AddShader(std::unique_ptr<Shader> spShader) override;
    virtual const std::vector<std::unique_ptr<RenderUnit>>& GetRenderUnits() const override {return _renderUnits;}
    virtual std::vector<std::unique_ptr<RenderUnit>>& GetRenderUnits() override {return _renderUnits;}
    void Render() override;
    const std::string& GetName() const override;

   private:
    std::vector<std::unique_ptr<RenderUnit>> _renderUnits;
    std::vector<std::unique_ptr<Shader>> _shader;
    std::string _name = "Canvas";
};

#endif  // CV_IMAGECANVAS_H
