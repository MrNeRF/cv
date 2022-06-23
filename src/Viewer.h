#ifndef VIEWER_H
#define VIEWER_H

#include <Eigen/Dense>
#include <memory>
#include <string>
#include <Renderer.h>

class Camera;
class Window;

class Viewer {
   public:
    Viewer(std::unique_ptr<Window> spWindow);
    int Init();
    void Run(void);
    void SetBackgroundColor(const Eigen::Vector4f& color) { _backgroundColor = color; }

   private:
    void render();

   private:
    std::unique_ptr<Window> _spWindow;
    std::shared_ptr<Camera> _spCamera;
    Renderer _renderer;
    Eigen::Vector4f _backgroundColor;
};

#endif
