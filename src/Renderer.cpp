//
// Created by paja on 6/26/22.
//

#include "Renderer.h"
void Renderer::Render() {
    for(auto& rData : _renderData) {
        rData.spRenderable->Render();
        rData.openglData.DrawObject(GL_TRIANGLES);
    }
}

void Renderer::AddRenderable(std::unique_ptr<IRenderable> spRenderObject) {
    _renderData.emplace_back();
    _renderData.back().spRenderable = std::move(spRenderObject);
    IRenderable* pRenderObject = _renderData.back().spRenderable.get();
    _renderData.back().openglData.InitializeBuffer(*pRenderObject->GetMesh());
    _renderData.back().openglData.InitializeTextureBuffer(pRenderObject->GetTexture());
}

void Renderer::RemoveRenderable(const std::string& rName) {
    uint32_t uiPos = 0;
    for(auto& rData : _renderData) {
        if (rData.spRenderable->GetName() == rName)
        {
            _renderData.erase(_renderData.begin() + uiPos);
        }
        else {
            ++uiPos;
        }
    }
}
