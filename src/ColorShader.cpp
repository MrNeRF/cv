//
// Created by paja on 6/23/22.
//

#include "ColorShader.h"
#include <filesystem>
#include "File.h"
#include "Light.h"
#include "Logger.h"
#include "Material.h"
#include "Model.h"

static const std::filesystem::path fragmentShaderName = "colorFS.glsl";
static const std::filesystem::path vertexShaderName = "colorVS.glsl";
static const std::string shaderName = "ColorShader";

ColorShader::ColorShader(const Eigen::Vector4f& rColor) : Shader(shaderName) {
    auto vertexShader = File(vertexShaderName, File::FileType::Shader);
    auto fragmentShader = File(fragmentShaderName, File::FileType::Shader);
    _color = rColor;
    initShaders(vertexShader, fragmentShader);
    SetColor(rColor);
}

void ColorShader::SetColor(const Eigen::Vector4f& rColor) {
    Shader::activateShader();
    // Pro tip: watch out upper/lower case (e.g. same as instance of struct in shader)
    SetVector("colorUniform", rColor);
}

void ColorShader::SetCamera(std::shared_ptr<Camera> spCamera) {
    if (spCamera == nullptr) {
        return;
    }
    _spCamera = spCamera;
}

void ColorShader::ActivateShader(const IRenderable* pRenderObject) {
    if (_spCamera == nullptr) {
        Logger::GetInstance().GetLogger().warn("Camera not set in shader {}", shaderName);
    }
    Shader::activateShader();

    SetVector("cameraPos", _spCamera->GetCameraPosition());
    SetTransformationMatrix("view", _spCamera->GetLookAt());
    SetTransformationMatrix("projection", _spCamera->GetPerspectiveProjection());

    const Model* pModel = dynamic_cast<const Model*>(pRenderObject);
    if (pModel != nullptr) {
        SetVector("translation", pModel->GetPosition());
    } else {
        ASSERT(0);
        Logger::GetInstance().GetLogger().error("IRenderable should be of type model.");
    }
}
