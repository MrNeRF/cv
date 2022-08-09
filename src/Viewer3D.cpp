#include "Viewer3D.h"
#include <GLFW/glfw3.h>
#include <memory>
#include "Camera.h"
#include "Importer.h"
#include "Light.h"
#include "Logger.h"
#include "Material.h"
#include <chrono>
#include "PhongShader.h"

int Viewer3D::Init(void) {
    _spCamera = std::make_shared<Camera>();
    _spCamera->SetPerspectiveProjection(45.f, _spWindow->GetAspectRatio(), 0.1, 50.f);
    return _spWindow->Init();
}

void Viewer3D::Run(void) {
    auto& rLogger = Logger::GetInstance().GetLogger();
    rLogger.info("Viewer3D::Run()");

    /* auto spModel = Importer::ImportModel("LibertyStatue/LibertStatue.obj"); */
    auto spModel = Importer::ImportModel("dice.obj");
    auto spPhongShader = std::make_unique<PhongShader>();

    spPhongShader->SetLightSource(std::make_shared<Light>());
    spPhongShader->SetMaterial(Material());
    spPhongShader->SetCamera(_spCamera);

    spModel->SetShader(std::move(spPhongShader));

    _renderer.AddRenderable(std::move(spModel));
    CHECK_GL_ERROR_(glEnable(GL_DEPTH_TEST));

    // std::vector<std::string> filenames = File("Shader").GetDirectoryContents();
    // std::vector<const char*> items;
    // std::transform(std::begin(filenames), std::end(filenames), std::back_inserter(items), std::me_fn(&std::string::c_str));
    render();
}

// RenderLoop
void Viewer3D::render() {

    uint32_t fps = 60;
    CHECK_GL_ERROR_(glClearColor(_backgroundColor.x() * _backgroundColor.w(),
                                 _backgroundColor.y() * _backgroundColor.w(),
                                 _backgroundColor.z() * _backgroundColor.w(),
                                 _backgroundColor.w()));

    CHECK_GL_ERROR_(glDepthFunc(GL_LESS))
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    while (!glfwWindowShouldClose(_spWindow->GetGLFWWindow())) {
        auto t1 = std::chrono::high_resolution_clock().now();
        CHECK_GL_ERROR_(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

        // UpdatePositions
        _spCamera->UpdateCameraPosition(_spWindow->GetCursorPostionDelta(), 1. / static_cast<double>(fps));
        // Draw
        _renderer.Render();
        glfwSwapBuffers(_spWindow->GetGLFWWindow());

        glfwPollEvents();
        auto t2 = std::chrono::high_resolution_clock().now();
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        fps = static_cast<uint32_t>(1000L / std::max(time_elapsed, 1L));
    }
}
