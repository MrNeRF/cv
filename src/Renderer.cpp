//
// Created by paja on 6/26/22.
//

#include "Renderer.h"

void Renderer::Render() {
    for (auto& rRenderObject : _renderingObjects) {
        rRenderObject->Render();
    }
}

void Renderer::AddRenderable(std::unique_ptr<IRenderable> spRenderObject) {
    _renderingObjects.push_back(std::move(spRenderObject));
}

void Renderer::RemoveRenderable(const std::string& rName) {
//    uint32_t uiPos = 0;
//    for (auto& rData : _renderData) {
//        if (rData.spRenderable->GetName() == rName) {
//            _renderData.erase(_renderData.begin() + uiPos);
//        } else {
//            ++uiPos;
//        }
//    }
}
void Renderer::UpdateTexture(const std::string& rName, const Texture* pTexture)
{
//    for (auto& rData : _renderData) {
//        if (rData.spRenderable->GetName() == rName) {
//            rData.openglData.UpdateTextureBuffer(pTexture);
//        }
//    }
}
