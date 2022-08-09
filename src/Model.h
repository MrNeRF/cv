#ifndef OBJECT3D_H

#define OBJECT3D_H

#include <Eigen/Dense>
#include <memory>
#include <string>
#include <vector>
#include "Camera.h"
#include "Eigen/src/Geometry/AngleAxis.h"
#include "Eigen/src/Geometry/Quaternion.h"
#include "IRenderable.h"
#include "Material.h"

class Shader;
struct Texture;
struct Mesh;

class Model : public IRenderable {
   public:
    [[nodiscard]] const Eigen::Vector4f& GetPosition() const { return _position; }
    [[nodiscard]] const Eigen::Quaternionf& GetOrientation() const { return _orientation; }
    Mesh* GetMesh() override { return _spMesh.get(); };
    void SetMaterial(std::unique_ptr<Material> spMaterial) { _spMaterial = std::move(spMaterial); };
    Material* GetMaterial() { return _spMaterial.get(); };
    Texture* GetTexture() override { return _spMaterial->spTexure.get();}

    void Render() override;
    [[nodiscard]] const std::string& GetName() const override { return _name; }
    void SetName(const std::string& name) {_name = name;}
    void SetMesh(std::unique_ptr<Mesh> spMesh) {_spMesh = std::move(spMesh);}

    void SetShader(std::unique_ptr<Shader> spShader) override;
    void SetOrientation(const Eigen::AngleAxisf& angleAxis) { _orientation = Eigen::Quaternionf(angleAxis) * _orientation; }

    void SetPosition(const Eigen::Vector4f& pos) { _position = pos; }
    void ResetRotation() { _orientation = Eigen::AngleAxis(0.f, Eigen::Vector3f::UnitX()); }

   private:
    std::unique_ptr<Mesh> _spMesh;
    std::unique_ptr<Material> _spMaterial;
    std::string _name;
    Eigen::Vector4f _position = Eigen::Vector4f(0.f, 0.f, 0.f, 1.f);
    Eigen::Quaternionf _orientation{Eigen::AngleAxis{0.f, Eigen::Vector3f::UnitX()}};
    std::unique_ptr<Texture> _spTexture;
    std::shared_ptr<Shader> _spShader;
};

#endif
