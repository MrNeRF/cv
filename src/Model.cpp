#include <GL/glew.h>
///
#include <numeric>
#include "File.h"
#include "Logger.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"

Model::Model(std::unique_ptr<Mesh> spMesh, const std::string& name)
    : _spMesh{std::move(spMesh)}, _name{name} {
    Logger::GetInstance().GetLogger().info("Create model: {}", _name);
}

void Model::Render() {
    ASSERT(_spShader != nullptr);
    _spShader->ActivateShader(this);
}
void Model::SetTexture(std::unique_ptr<Texture> spTexture) {
    ASSERT(spTexture != nullptr);
    _spTexture = std::move(spTexture);
}

void Model::SetShader(std::unique_ptr<Shader> spShader) {
    ASSERT(spShader != nullptr);
    _spShader = std::move(spShader);
}