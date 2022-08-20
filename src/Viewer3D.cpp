#include "Viewer3D.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <memory>
#include "Camera.h"
#include "ColorShader.h"
#include "Importer.h"
#include "Light.h"
#include "Logger.h"
#include "Model.h"
#include "PhongShader.h"
#include "Primitives3D.h"

int Viewer3D::Init(void) {
    _spLight = std::make_shared<Light>();
    _spCamera = std::make_shared<Camera>();
    _spCamera->SetPerspectiveProjection(45.f, _spWindow->GetAspectRatio(), 0.1, 50.f);
    _spWindow->RegisterObserver(_spCamera, InputEvent::InputEventType::MouseWheel);
    return _spWindow->Init();
}

void Viewer3D::Run(void) {
    auto& rLogger = Logger::GetInstance().GetLogger();
    rLogger.info("Viewer3D::Run()");

    {
        auto spModel = Importer::ImportModel("racecar.obj");
        //        auto spModel = Importer::ImportModel("dice.obj");

//        Eigen::Matrix3f scale;
//        scale(0, 0) = 5.f;
//        scale(1, 1) = 5.f;
//        scale(2, 2) = 5.f;
//        dynamic_cast<Model*>(spModel.get())->Transform(scale);
        auto& renderUnits = spModel->GetRenderUnits();
        for (auto& renderUnit : renderUnits) {
            auto spPhongShader = std::make_unique<PhongShader>();
            spPhongShader->SetLightSource(_spLight);
            spPhongShader->SetMaterial(*renderUnit->pMaterial);
            spPhongShader->SetCamera(_spCamera);
            spPhongShader->SetName(renderUnit->pMaterial->materialName);
            renderUnit->pShader = spModel->AddShader(std::move(spPhongShader));
            renderUnit->spOpenGLData = std::make_unique<OpenGL3DDataObject>();

            renderUnit->InitializeRenderData();
            //            auto pModel = dynamic_cast<Model*>(spModel.get());
            //            pModel->SetPosition(Eigen::Vector4f(0.f, 1.f, 0.f, 1.f));
        }
        _renderer.AddRenderable(std::move(spModel));
    }

    {
        auto spModel = std::make_unique<Model>();
        auto spRenderUnit = std::make_unique<RenderUnit>();
        spRenderUnit->spMesh = Primitives3D::CreateCube(1.f, 1.f, 1.f);
        spModel->AddRenderUnit(std::move(spRenderUnit));
        auto& renderUnits = spModel->GetRenderUnits();
        for (auto& renderUnit : renderUnits) {
            auto spShader = std::make_unique<ColorShader>(Eigen::Vector4f(1.f, 1.f, 1.f, 1.f));
            spShader->SetCamera(_spCamera);
            spShader->SetName("Light");
            renderUnit->pShader = spModel->AddShader(std::move(spShader));
            renderUnit->spOpenGLData = std::make_unique<OpenGL3DDataObject>();
            renderUnit->InitializeRenderData();
            //            auto pModel = dynamic_cast<Model*>(spModel.get());
            //            pModel->SetPosition(Eigen::Vector4f(0.f, 1.f, 0.f, 1.f));
        }
        spModel->SetPosition(_spLight->GetPostion());
        _renderer.AddRenderable(std::move(spModel));
    }

    //    {
    //        auto spModel2 = Importer::ImportModel("dice.obj");
    //        auto& renderUnits2 = spModel2->GetRenderUnits();
    //        for(auto &renderUnit : renderUnits2) {
    //            auto spPhongShader = std::make_unique<PhongShader>();
    //            spPhongShader->SetLightSource(std::make_shared<Light>());
    //            spPhongShader->SetMaterial(*renderUnit->pMaterial);
    //            spPhongShader->SetCamera(_spCamera);
    //            spPhongShader->SetName(renderUnit->pMaterial->materialName);
    //            renderUnit->pShader = spModel2->AddShader(std::move(spPhongShader));
    //            renderUnit->spOpenGLData = std::make_unique<OpenGL3DDataObject>();
    //            renderUnit->InitializeRenderData();
    //            auto pModel = dynamic_cast<Model*>(spModel2.get());
    //            pModel->SetPosition(Eigen::Vector4f(0.f, -1.f, 0.f, 1.f));
    //        }
    //        _renderer.AddRenderable(std::move(spModel2));
    //    }

    CHECK_GL_ERROR_(glEnable(GL_DEPTH_TEST));
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
        auto t1 = std::chrono::high_resolution_clock::now();
        CHECK_GL_ERROR_(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))
        _spWindow->HandleInputEvent();

        // UpdatePositions
        _spCamera->UpdateCameraPosition(_spWindow->GetCursorPostionDelta(), 1. / static_cast<double>(fps));
        // Draw
        _renderer.Render();
        glfwSwapBuffers(_spWindow->GetGLFWWindow());

        glfwPollEvents();  // should be invoked ater glfwSwapBuffers
        auto t2 = std::chrono::high_resolution_clock::now();
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        fps = static_cast<uint32_t>(1000L / std::max(time_elapsed, 1L));
    }
}
