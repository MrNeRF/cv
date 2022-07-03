#include "ImageViewer.h"
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "CanvasShader.h"
#include "ImageCanvas.h"
#include "Importer.h"
#include "Light.h"
#include "Logger.h"
#include "Material.h"
#include "Window.h"

int Viewer3D::Init(void) {
    _spCamera = std::make_shared<Camera>();
    _spCamera->SetPerspectiveProjection(45.f, _spWindow->GetAspectRatio(), 0.1, 50.f);
    return _spWindow->Init();
}

void Viewer3D::Run(void) {
    auto& rLogger = Logger::GetInstance().GetLogger();
    rLogger.info("Viewer3D::Run()");

    auto spCanvasShader = std::make_unique<CanvasShader>();
    spCanvasShader->SetCamera(_spCamera);

    auto spCanvas = std::make_unique<ImageCanvas>();
    spCanvas->SetShader(std::move(spCanvasShader));

    _renderer.AddRenderable(std::move(spCanvas));
    CHECK_GL_ERROR_(glEnable(GL_DEPTH_TEST));

    // std::vector<std::string> filenames = File("Shader").GetDirectoryContents();
    // std::vector<const char*> items;
    // std::transform(std::begin(filenames), std::end(filenames), std::back_inserter(items), std::me_fn(&std::string::c_str));
    render();
}

// RenderLoop
void Viewer3D::render() {

    CHECK_GL_ERROR_(glClearColor(_backgroundColor.x() * _backgroundColor.w(),
                                 _backgroundColor.y() * _backgroundColor.w(),
                                 _backgroundColor.z() * _backgroundColor.w(),
                                 _backgroundColor.w()));
    CHECK_GL_ERROR_(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

    CHECK_GL_ERROR_(glDepthFunc(GL_LESS))
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // UpdatePositions
    _spCamera->UpdateCameraPosition(_spWindow->GetLastDirection());
    // Draw
    _renderer.Render();
    glfwSwapBuffers(_spWindow->GetGLFWWindow());

    while (!glfwWindowShouldClose(_spWindow->GetGLFWWindow())) {
        glfwPollEvents();
    }
}
