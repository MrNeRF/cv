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

static const std::filesystem::path fragmentShaderName = "phongFS.glsl";
static const std::filesystem::path vertexShaderName = "phongVS.glsl";
static const std::string shaderName = "PhongShader";

PhongShader::PhongShader() : Shader(shaderName) {
    auto vertexShader = File(vertexShaderName, File::FileType::Shader);
    auto fragmentShader = File(fragmentShaderName, File::FileType::Shader);
    initShaders(vertexShader, fragmentShader);
}

void PhongShader::SetMaterial(const Material& rMaterial) {
    Shader::activateShader();
    // Pro tip: watch out upper/lower case (e.g. same as instance of struct in shader)
    SetVector("material.ambient", rMaterial.ambient);
    SetVector("material.diffuse", rMaterial.diffuse);
    SetVector("material.specular", rMaterial.specular);
    SetValue("material.specularExponent", rMaterial.specularExponent);
}

void PhongShader::SetLightSource(Light* pLight) {
    if (pLight == nullptr) {
        return;
    }
    Shader::activateShader();
    _pLight = pLight;
    SetVector("light.ambient", _pLight->GetAmbient());
    SetVector("light.diffuse", _pLight->GetDiffuse());
    SetVector("light.specular", _pLight->GetSpecular());
    SetVector("light.position", _pLight->GetPostion());
}

void PhongShader::SetCamera(std::shared_ptr<Camera> spCamera) {
    if (spCamera == nullptr) {
        return;
    }
    _spCamera = spCamera;
}

void PhongShader::ActivateShader(const IRenderable* pRenderObject) {
    if (_pLight == nullptr) {
        Logger::GetInstance().GetLogger().warn("Light not set in shader {}", shaderName);
    }
    if (_spCamera == nullptr) {
        Logger::GetInstance().GetLogger().warn("Camera not set in shader {}", shaderName);
    }
    Shader::activateShader();

    SetVector("cameraPos", _spCamera->GetCameraPosition());
    SetTransformationMatrix("view", _spCamera->GetLookAt());
    SetTransformationMatrix("projection", _spCamera->GetPerspectiveProjection());

    const Model* pModel = dynamic_cast<const Model*>(pRenderObject);
    if (pModel != nullptr) {
        SetQuat("transform.qconjOrientation", pModel->GetOrientation().conjugate());
        SetQuat("transform.qOrientation", pModel->GetOrientation());
        SetTransformationMatrix("normalRotationMatrix", pModel->GetOrientation().toRotationMatrix());
        SetVector("transform.position", pModel->GetPosition());
    } else {
        ASSERT(0);
        Logger::GetInstance().GetLogger().error("IRenderable should be of type model.");
    }
}
