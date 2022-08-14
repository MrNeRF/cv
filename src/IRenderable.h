#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <memory>
#include <vector>
#include <string>
#include "RenderUnit.h"

class Shader;

class IRenderable {
   public:
    virtual ~IRenderable() = default;
    virtual const Shader* AddShader(std::unique_ptr<Shader> spShader) = 0;
    virtual void Render() = 0;
    virtual const std::vector<std::unique_ptr<RenderUnit>>& GetRenderUnits() const = 0;
    virtual std::vector<std::unique_ptr<RenderUnit>>& GetRenderUnits() = 0;
    virtual const std::string& GetName() const = 0;
};
#endif
