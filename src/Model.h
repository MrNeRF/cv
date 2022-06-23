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

class Shader;
struct Texture;
struct Mesh;

class Model : public IRenderable {
   public:
    explicit Model(std::unique_ptr<Mesh> spMesh, const std::string& name);
    [[nodiscard]] const Eigen::Vector4f& GetPosition() const { return _position; }
    [[nodiscard]] const Eigen::Quaternionf& GetOrientation() const { return _orientation; }
    Mesh* GetMesh() override {return _spMesh.get();};
    Texture* GetTexture() override {return _spTexture.get();};

    void Render() override;
    [[nodiscard]] const std::string& GetName() const override { return _name; }
    void SetTexture(std::unique_ptr<Texture> spTexture) override;
    void SetShader(std::unique_ptr<Shader> spShader) override;
    void SetOrientation(const Eigen::AngleAxisf& angleAxis){_orientation = Eigen::Quaternionf(angleAxis) * _orientation;}

    void SetPosition(const Eigen::Vector4f& pos) { _position = pos; }
    void ResetRotation() { _orientation = Eigen::AngleAxis(0.f, Eigen::Vector3f::UnitX()); }

   private:
    std::unique_ptr<Mesh> _spMesh;
    const std::string _name;
    Eigen::Vector4f _position = Eigen::Vector4f(2.f, 3.f, -5.f, 1.f);
    Eigen::Quaternionf _orientation{Eigen::AngleAxis{0.f, Eigen::Vector3f::UnitX()}};
    std::unique_ptr<Texture> _spTexture;
    std::shared_ptr<Shader> _spShader;
};

#endif
