#include <GL/glew.h>
///
#include <numeric>
#include "File.h"
#include "Logger.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"

void Model::Render() {
    ASSERT(_spShader != nullptr);
    _spShader->ActivateShader(this);
}

void Model::SetShader(std::unique_ptr<Shader> spShader) {
    ASSERT(spShader != nullptr);
    _spShader = std::move(spShader);
}