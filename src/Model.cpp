#include <GL/glew.h>
///
#include <numeric>
#include "File.h"
#include "Logger.h"
#include "Model.h"
#include "Shader.h"

void Model::Render() {
    ASSERT(_renderUnits.size() == _shaders.size());
    size_t maxVertices = 0;
    int pos = 0;
    int maxPos = 0;
    for(auto &spRenderUnit : _renderUnits) {
        if(spRenderUnit->spMesh->vertices.size() > maxVertices) {
            maxVertices = spRenderUnit->spMesh->vertices.size();
            maxPos = pos;
        }
        ++pos;
    }
    pos = 0;
    maxPos = 0;
    for(auto &spShader : _shaders) {
        if(pos == maxPos) {
            spShader->ActivateShader(this);
        }
        ++pos;
    }
}

const Shader* Model::AddShader(std::unique_ptr<Shader> spShader) {
    ASSERT(spShader != nullptr);
    _shaders.push_back(std::move(spShader));
    return _shaders.back().get();
}

const Material* Model::GetMaterial(const std::string& rMaterialName) const {
    for (const auto& spMaterial : _materials) {
        return spMaterial.get();
    }
    return nullptr;
}
