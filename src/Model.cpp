#include <GL/glew.h>
///
#include <numeric>
#include "File.h"
#include "Logger.h"
#include "Model.h"
#include "Shader.h"

void Model::Render() {
    for (auto& spRenderUnit : _renderUnits) {
        spRenderUnit->pShader->ActivateShader(this);
        spRenderUnit->spOpenGLData->DrawObject(GL_TRIANGLES);
    }
}

Shader* Model::AddShader(std::unique_ptr<Shader> spShader) {
    ASSERT(spShader != nullptr);
    _shaders.push_back(std::move(spShader));
    return _shaders.back().get();
}

const Material* Model::GetMaterial(const std::string& rMaterialName) const {
    for (const auto& spMaterial : _materials) {
        if (spMaterial->materialName == rMaterialName) {
            return spMaterial.get();
        }
    }
    return nullptr;
}

void Model::CreateBoundingVolumes(Primitives::Types bvType) {
    for (auto& spRenderUnit : _renderUnits) {
        spRenderUnit->bv = collision::BoundingVolume(bvType, *spRenderUnit->spMesh);
    }
}

void Model::AddRenderUnit(std::unique_ptr<RenderUnit> spRenderUnit) {
    _renderUnits.push_back(std::move(spRenderUnit));
}

void Model::Transform(const Eigen::Matrix3f& rTransformation) {
    for (auto& spRenderUnit : _renderUnits) {
        auto& rVertices = spRenderUnit->spMesh->vertices;
        for (auto& rVertex : rVertices) {
            rVertex.position = rTransformation * rVertex.position;
        }
    }
}

void Model::CheckCollision(const Camera* pCamera) {
    for (auto& spRenderUnit : _renderUnits) {
        Eigen::Vector3f pos;
        pos.x() = _position.x();
        pos.y() = _position.y();
        pos.z() = _position.y();
        collision::MeshRayCollision(*spRenderUnit->spMesh, pCamera->GetCameraRay(), pos, _orientation.toRotationMatrix(), spRenderUnit->bv);
    }
}
