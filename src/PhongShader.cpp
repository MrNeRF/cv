//
// Created by paja on 6/23/22.
//

#include "PhongShader.h"
#include <filesystem>
#include "File.h"
#include "Light.h"
#include "Logger.h"
#include "Material.h"
#include "Model.h"

static const std::filesystem::path fragmentShaderName = "shader.fs";
static const std::filesystem::path vertexShaderName = "shader.vs";
static const std::string shaderName = "PhongShader";

PhongShader::PhongShader() : Shader(shaderName) {
    auto vertexShader = File(vertexShaderName, File::FileType::Shader);
    auto fragmentShader = File(fragmentShaderName, File::FileType::Shader);
    initShaders(vertexShader, fragmentShader);
}

void PhongShader::SetMaterial(const Material& rMaterial) {
    Shader::activateShader();
    _spMaterial = std::make_unique<Material>(rMaterial);
    SetVector("Material.ambient", _spMaterial->GetAmbient());
    SetVector("Material.diffuse", _spMaterial->GetDiffuse());
    SetVector("Material.specular", _spMaterial->GetSpecular());
    SetValue("Material.shininess", _spMaterial->GetShininess());
}

void PhongShader::SetLightSource(std::shared_ptr<Light> spLight) {
    if (spLight == nullptr) {
        return;
    }
    Shader::activateShader();
    _spLight = spLight;
    SetVector("light.ambient", _spLight->GetAmbient());
    SetVector("light.diffuse", _spLight->GetDiffuse());
    SetVector("light.specular", _spLight->GetSpecular());
    SetVector("light.position", _spLight->GetPostion());
}

void PhongShader::SetCamera(std::shared_ptr<Camera> spCamera) {
    if (spCamera == nullptr) {
        return;
    }
    _spCamera = spCamera;
}

void PhongShader::ActivateShader(const Model* pModel) {
    if (_spLight == nullptr) {
        Logger::GetInstance().GetLogger().warn("Light not set in shader {}", shaderName);
    }
    if (_spMaterial == nullptr) {
        Logger::GetInstance().GetLogger().warn("Material not set in shader {}", shaderName);
    }
    if (_spCamera == nullptr) {
        Logger::GetInstance().GetLogger().warn("Camera not set in shader {}", shaderName);
    }
    Shader::activateShader();

    SetVector("cameraPos", _spCamera->GetCameraPosition());
    SetTransformationMatrix("view", _spCamera->GetLookAt());
    SetTransformationMatrix("projection", _spCamera->GetPerspectiveProjection());

    SetQuat("transform.qconjOrientation", pModel->GetOrientation().conjugate());
    SetQuat("transform.qOrientation", pModel->GetOrientation());
    SetTransformationMatrix("normalRotationMatrix", pModel->GetOrientation().toRotationMatrix());
    SetVector("transform.position", pModel->GetPosition());
}
