#include <GL/glew.h>
///
#include <numeric>
#include "File.h"
#include "Logger.h"
#include "Model.h"
#include "Shader.h"

void Model::Render() {
    ASSERT(_renderUnits.size() == _shaders.size());
    for(auto &spShader : _shaders) {
        spShader->ActivateShader(this);
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
