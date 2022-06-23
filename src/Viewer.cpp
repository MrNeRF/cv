#include "Viewer.h"
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Light.h"
#include "Logger.h"
#include "Material.h"
#include "Importer.h"
#include "PhongShader.h"
#include "Window.h"
#include "Camera.h"

Viewer::Viewer(std::unique_ptr<Window> spWindow) : _spWindow{std::move(spWindow)} {
}

int Viewer::Init(void) {
    _spCamera = std::make_shared<Camera>();
    _spCamera->SetPerspectiveProjection(45.f, _spWindow->GetAspectRatio(), 0.1, 50.f);
    return _spWindow->Init();
}

void Viewer::Run(void) {
    auto& rLogger = Logger::GetInstance().GetLogger();
    rLogger.info("Viewer::Run()");

    auto spModel = Importer::ImportModel("dice.obj");
    auto spPhongShader = std::make_unique<PhongShader>();

    spPhongShader->SetLightSource(std::make_shared<Light>());
    spPhongShader->SetMaterial(Material());
    spPhongShader->SetCamera(_spCamera);

    spModel->SetShader(std::move(spPhongShader));

    _renderer.AddRenderable(std::move(spModel));
    CHECK_GL_ERROR_(glEnable(GL_DEPTH_TEST));

    //std::vector<std::string> filenames = File("Shader").GetDirectoryContents();
    //std::vector<const char*> items;
    //std::transform(std::begin(filenames), std::end(filenames), std::back_inserter(items), std::me_fn(&std::string::c_str));
    render();
}

// RenderLoop
void Viewer::render() {
    while (!glfwWindowShouldClose(_spWindow->GetGLFWWindow())) {
        glfwPollEvents();

        CHECK_GL_ERROR_(glClearColor(_backgroundColor.x() * _backgroundColor.w(),
                                     _backgroundColor.y() * _backgroundColor.w(),
                                     _backgroundColor.z() * _backgroundColor.w(),
                                     _backgroundColor.w()));
        CHECK_GL_ERROR_(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

        CHECK_GL_ERROR_(glDepthFunc(GL_LESS))
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // UpdatePositions
        // Draw
        _renderer.Render();
        glfwSwapBuffers(_spWindow->GetGLFWWindow());
    }
}
