//
// Created by paja on 6/26/22.
//

#include "Renderer.h"
#include "Model.h"

void Renderer::Render() {
    for (auto& rData : _renderData) {
        rData.spRenderable->Render();
        rData.openglData.DrawObject(GL_TRIANGLES);
    }
}

void Renderer::AddRenderable(std::unique_ptr<IRenderable> spRenderObject) {
    _renderData.emplace_back();
    _renderData.back().spRenderable = std::move(spRenderObject);
    IRenderable* pRenderObject = _renderData.back().spRenderable.get();
    const auto& renderUnits = pRenderObject->GetRenderUnits();
    for(const auto &spRenderUnit : renderUnits) {
        _renderData.back().openglData.InitializeBuffer(spRenderUnit.get());
    }
}

void Renderer::RemoveRenderable(const std::string& rName) {
    uint32_t uiPos = 0;
    for (auto& rData : _renderData) {
        if (rData.spRenderable->GetName() == rName) {
            _renderData.erase(_renderData.begin() + uiPos);
        } else {
            ++uiPos;
        }
    }
}
void Renderer::UpdateTexture(const std::string& rName, const Texture* pTexture)
{
    for (auto& rData : _renderData) {
        if (rData.spRenderable->GetName() == rName) {
            rData.openglData.UpdateTextureBuffer(pTexture);
        }
    }
}
