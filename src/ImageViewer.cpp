#include "ImageViewer.h"
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "CanvasShader.h"
#include "Image.h"
#include "ImageCanvas.h"
#include "Importer.h"
#include "Light.h"
#include "Logger.h"
#include "Material.h"
#include "Window.h"

int ImageViewer::Init(void) {
    _spCamera = std::make_shared<Camera>();
    _spCamera->SetPerspectiveProjection(45.f, _spWindow->GetAspectRatio(), 0.1, 50.f);
    return _spWindow->Init();
}

void ImageViewer::ShowImage(const Image& rImage) {
    auto& rLogger = Logger::GetInstance().GetLogger();
    rLogger.info("Viewer3D::ShowImage()");

    auto spCanvasShader = std::make_unique<CanvasShader>();
    spCanvasShader->SetCamera(_spCamera);

    auto spCanvas = std::make_unique<ImageCanvas>();
    spCanvas->SetShader(std::move(spCanvasShader));
    auto spTexture = std::make_unique<Texture>(rImage);
    spCanvas->SetTexture(std::move(spTexture));

    _renderer.AddRenderable(std::move(spCanvas));
    CHECK_GL_ERROR_(glEnable(GL_DEPTH_TEST));
    render();
}

// RenderLoop
void ImageViewer::render() {

    CHECK_GL_ERROR_(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))
    CHECK_GL_ERROR_(glDepthFunc(GL_LESS))
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    _renderer.Render();
    glfwSwapBuffers(_spWindow->GetGLFWWindow());
    while (!glfwWindowShouldClose(_spWindow->GetGLFWWindow())) {

        // Draw
        glfwPollEvents();
    }
}