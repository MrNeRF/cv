//
// Created by paja on 6/26/22.
//

#ifndef CV_RENDERER_H
#define CV_RENDERER_H

#include <memory>
#include <vector>
#include "IRenderable.h"
#include "OpenGL3DDataObject.h"

struct Texture;

class Renderer {
   public:
    void Render();
    void AddRenderable(std::unique_ptr<IRenderable> spRenderObject);
    void RemoveRenderable(const std::string& rName);
    void UpdateTexture(const std::string& rName, const Texture* pTexture);

   private:
    std::vector<std::unique_ptr<IRenderable>> _renderingObjects;
};

#endif  // CV_RENDERER_H
