//
// Created by paja on 7/3/22.
//

#include "CanvasShader.h"
#include "File.h"
#include "IRenderable.h"

static const std::filesystem::path fragmentShaderName = "imageCanvasFS.shader";
static const std::filesystem::path vertexShaderName = "imageCanvasVS.shader";
static const std::string shaderName = "ImageCanvasShader";

CanvasShader::CanvasShader() : Shader(shaderName) {
    auto vertexShader = File(vertexShaderName, File::FileType::Shader);
    auto fragmentShader = File(fragmentShaderName, File::FileType::Shader);
    initShaders(vertexShader, fragmentShader);
}

void CanvasShader::ActivateShader(const IRenderable* pRenderObject) {
    if (_spCamera == nullptr) {
        Logger::GetInstance().GetLogger().warn("Camera not set in shader {}", shaderName);
    }
    Shader::activateShader();
}

void CanvasShader::SetCamera(std::shared_ptr<Camera> spCamera) {
    if (spCamera == nullptr) {
        return;
    }
    _spCamera = spCamera;
}
