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
#include "RenderUnit.h"

class Shader;

class Model : public IRenderable {
   public:
    [[nodiscard]] const Eigen::Vector4f& GetPosition() const { return _position; }
    [[nodiscard]] const Eigen::Quaternionf& GetOrientation() const { return _orientation; }
    const std::vector<std::unique_ptr<RenderUnit>>& GetRenderUnits() const override { return _renderUnits; };
    std::vector<std::unique_ptr<RenderUnit>>& GetRenderUnits() override { return _renderUnits; };
    void AddMaterial(std::unique_ptr<Material> spMaterial) { _materials.push_back(std::move(spMaterial)); };
    const Material* GetMaterial(const std::string& rMaterialName) const;

    void Render() override;
    [[nodiscard]] const std::string& GetName() const override { return _name; }
    void SetName(const std::string& name) {_name = name;}
    void AddRenderUnit(std::unique_ptr<RenderUnit> spRenderUnit);

    Shader* AddShader(std::unique_ptr<Shader> spShader) override;
    void SetOrientation(const Eigen::AngleAxisf& angleAxis) { _orientation = Eigen::Quaternionf(angleAxis) * _orientation; }

    void SetPosition(const Eigen::Vector4f& pos) { _position = pos; }
    void ResetRotation() { _orientation = Eigen::AngleAxis(0.f, Eigen::Vector3f::UnitX()); }

   private:
    std::vector<std::unique_ptr<RenderUnit>> _renderUnits;
    std::vector<std::unique_ptr<Material>> _materials;
    std::vector<std::unique_ptr<Shader>> _shaders;

    std::string _name;
    Eigen::Vector4f _position = Eigen::Vector4f(0.f, 0.f, 0.f, 1.f);
    Eigen::Quaternionf _orientation{Eigen::AngleAxis{0.f, Eigen::Vector3f::UnitX()}};
};

#endif
