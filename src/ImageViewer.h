#ifndef IMAGE_VIEWER_H
#define IMAGE_VIEWER_H

#include <Renderer.h>
#include <Window.h>
#include <Eigen/Dense>
#include <memory>
#include <string>
#include "Viewer.h"

class Camera;
class Image;

class ImageViewer : public Viewer {
   public:
    ImageViewer(const std::string windowName) : _spWindow{std::make_unique<Window>(windowName, 600, 800)} {}
    int Init() override;
    void ShowImage(const Image& rImage);

   private:
    void render();

   private:
    std::unique_ptr<Window> _spWindow;
    std::shared_ptr<Camera> _spCamera;
    Renderer _renderer;
};

#endif
