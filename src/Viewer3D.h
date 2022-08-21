#ifndef VIEWER_H
#define VIEWER_H

#include <Renderer.h>
#include <Window.h>
#include <Eigen/Dense>
#include <memory>
#include <string>
#include "SceneGraph.h"
#include "Viewer.h"

class Camera;
class Light;

class Viewer3D : public Viewer {
   public:
    explicit Viewer3D(const std::string& windowName) : _spWindow{std::make_unique<Window>(windowName)} {}
    int Init() override;
    void Run();
    void SetBackgroundColor(const Eigen::Vector4f& color) { _backgroundColor = color; }

   private:
    void render();

   private:
    std::unique_ptr<scene::SceneGraph> _spSceneGraph;
    std::shared_ptr<Light> _spLight;
    std::unique_ptr<Window> _spWindow;
    std::shared_ptr<Camera> _spCamera;
    std::vector<std::unique_ptr<IRenderable>> _renderObjects;
    Renderer _renderer;
    Eigen::Vector4f _backgroundColor;
};

#endif
