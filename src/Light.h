#ifndef CV_LIGHT_H
#define CV_LIGHT_H

#include <Eigen/Dense>
#include <utility>
#include "IRenderable.h"

class Light : public IRenderable {
   public:
    Light(const std::string& rName);
    Light(std::string name,
          Eigen::Vector3f ambient,
          Eigen::Vector3f diffuse,
          Eigen::Vector3f specular,
          Eigen::Vector4f position);
    virtual ~Light() = default;
    virtual Shader* AddShader(std::unique_ptr<Shader> spShader) override;
    void Render() override;
    const std::vector<std::unique_ptr<RenderUnit>>& GetRenderUnits() const override { return _renderUnits; };
    std::vector<std::unique_ptr<RenderUnit>>& GetRenderUnits() override { return _renderUnits; };
    const std::string& GetName() const override { return _name; };
    [[nodiscard]] const Eigen::Vector3f& GetAmbient() const { return _ambient; }
    [[nodiscard]] const Eigen::Vector3f& GetDiffuse() const { return _diffuse; }
    [[nodiscard]] const Eigen::Vector3f& GetSpecular() const { return _specular; }
    [[nodiscard]] const Eigen::Vector4f& GetPostion() const { return _position; }

   private:
    const std::string& _name;
    std::unique_ptr<Shader> _spShader;
    std::vector<std::unique_ptr<RenderUnit>> _renderUnits;
    Eigen::Vector3f _ambient{.5f, .5f, .5f};
    Eigen::Vector3f _diffuse{.2f, .2f, .2f};
    Eigen::Vector3f _specular{.2f, .2f, .2f};
    Eigen::Vector4f _position{1.f, 2.f, 6.f, 1.f};
};
#endif  // CV_LIGHT_H
