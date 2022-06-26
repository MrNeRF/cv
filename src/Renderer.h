//
// Created by paja on 6/26/22.
//

#ifndef CV_RENDERER_H
#define CV_RENDERER_H

#include <memory>
#include <vector>
#include "IRenderable.h"
#include "OpenGL3DDataObject.h"

class Model;

class Renderer {
   private:
    struct RenderData {
        std::shared_ptr<IRenderable> spRenderable;
        OpenGL3DDataObject openglData;
    };

   public:
    void Render();
    void AddRenderable(std::unique_ptr<IRenderable> spRenderObject);
    void RemoveRenderable(const std::string& rName);

   private:
    std::vector<RenderData> _renderData;
};

#endif  // CV_RENDERER_H
