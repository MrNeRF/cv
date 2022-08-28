//
// Created by paja on 6/23/22.
//

#include "GridFloorShader.h"
#include <filesystem>
#include "File.h"
#include "Light.h"
#include "Logger.h"
#include "Model.h"

static const std::filesystem::path fragmentShaderName = "viewPortGridFS.glsl";
static const std::filesystem::path vertexShaderName = "viewPortGridVS.glsl";
static const std::string shaderName = "GridFloorShader";

GridFloorShader::GridFloorShader() : Shader(shaderName) {
    auto vertexShader = File(vertexShaderName, File::FileType::Shader);
    auto fragmentShader = File(fragmentShaderName, File::FileType::Shader);
    initShaders(vertexShader, fragmentShader);
}

void GridFloorShader::SetCamera(std::shared_ptr<Camera> spCamera) {
    if (spCamera == nullptr) {
        return;
    }
    _spCamera = spCamera;
}

void GridFloorShader::ActivateShader(const IRenderable* pRenderObject) {
    if (_spCamera == nullptr) {
        Logger::GetInstance().GetLogger().warn("Camera not set in shader {}", shaderName);
    }
    Shader::activateShader();

    SetValue("farValIn", _spCamera->GetFarPlaneValue());
    SetValue("nearValIn", _spCamera->GetNearPlaneValue());
    SetTransformationMatrix("view", _spCamera->GetLookAt());
    SetTransformationMatrix("projection", _spCamera->GetPerspectiveProjection());
}
