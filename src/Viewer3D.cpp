#include "Viewer3D.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <memory>
#include "Camera.h"
#include "ColorShader.h"
#include "GridFloorShader.h"
#include "Importer.h"
#include "Light.h"
#include "Logger.h"
#include "Model.h"
#include "PhongShader.h"
#include "Primitives.h"
#include "Primitives3D.h"

int Viewer3D::Init(void) {
    _spSceneGraph = std::make_unique<scene::SceneGraph>();
    auto spNodeGroup = std::make_unique<scene::Node>("LightGroup");
    scene::Node* pParentNode = _spSceneGraph->InserNode(nullptr, std::move(spNodeGroup));

    auto spNode = std::make_unique<scene::Node>("Light");
    spNode->renderable = std::make_unique<Light>("Light");
    spNode->pParent = pParentNode;
    _spSceneGraph->InserNode(pParentNode, std::move(spNode));

    spNode = std::make_unique<scene::Node>("CameraGroup");
    pParentNode = _spSceneGraph->InserNode(nullptr, std::move(spNode));

    spNode = std::make_unique<scene::Node>("ModelGroup");
    pParentNode = _spSceneGraph->InserNode(nullptr, std::move(spNode));

    _spCamera = std::make_shared<Camera>();
    _spCamera->SetPerspectiveProjection(45.f, _spWindow->GetAspectRatio(), 0.01, 100.f);
    _spWindow->RegisterObserver(_spCamera, InputEvent::InputEventType::MouseWheel);
    _spWindow->RegisterObserver(_spCamera, InputEvent::InputEventType::MouseButton);
    _spWindow->RegisterObserver(_spCamera, InputEvent::InputEventType::WindowResize);
    _spCamera->SetWindowSize(_spWindow->_winWidth, _spWindow->_winHeight);
    return _spWindow->Init();
}

void Viewer3D::Run(void) {
    auto& rLogger = Logger::GetInstance().GetLogger();
    rLogger.info("Viewer3D::Run()");
    {
        auto spModel = Importer::ImportModel("racecar.obj");
        auto& renderUnits = spModel->GetRenderUnits();
        const Eigen::Affine3f rotation(Eigen::AngleAxis<float>(-std::numbers::pi_v<float> * 0.5f, Eigen::Vector3f::UnitX()));
        auto pModel = dynamic_cast<Model*>(spModel.get());
        pModel->Transform(rotation.rotation());
        pModel->SetPosition(Eigen::Vector4f(0.f, .0f, 0.f, 1.f));
        for (auto& renderUnit : renderUnits) {
            auto spPhongShader = std::make_unique<PhongShader>();
            spPhongShader->SetLightSource(dynamic_cast<Light*>(_spSceneGraph->GetNode("Light")->renderable.get()));
            spPhongShader->SetMaterial(*renderUnit->pMaterial);
            spPhongShader->SetCamera(_spCamera);
            spPhongShader->SetName(renderUnit->pMaterial->materialName);
            renderUnit->pShader = spModel->AddShader(std::move(spPhongShader));
            renderUnit->spOpenGLData = std::make_unique<OpenGL3DDataObject>();
            renderUnit->InitializeRenderData();
        }
        dynamic_cast<Model*>(spModel.get())->CreateBoundingVolumes(Primitives::Types::Cuboid);
        dynamic_cast<Model*>(spModel.get())->SetName("RaceCar");

        _renderer.AddRenderable(spModel.get());

        _renderObjects.push_back(std::move(spModel));
    }

    {
        auto spModel = std::make_unique<Model>();
        auto spRenderUnit = std::make_unique<RenderUnit>();
        spRenderUnit->spMesh = algorithm::PrimitiveToMesh(Cuboid());
        spModel->AddRenderUnit(std::move(spRenderUnit));
        auto pModel = dynamic_cast<Model*>(spModel.get());
        pModel->SetPosition(Eigen::Vector4f(0.f, 4.f, 2.f, 1.f));
        auto& renderUnits = spModel->GetRenderUnits();
        for (auto& renderUnit : renderUnits) {
            auto spShader = std::make_unique<ColorShader>(Eigen::Vector4f(1.f, 1.f, 1.f, 1.f));
            spShader->SetCamera(_spCamera);
            spShader->SetName("Light");
            renderUnit->pShader = spModel->AddShader(std::move(spShader));
            renderUnit->spOpenGLData = std::make_unique<OpenGL3DDataObject>();
            renderUnit->InitializeRenderData();
        }
        dynamic_cast<Model*>(spModel.get())->CreateBoundingVolumes(Primitives::Types::Cuboid);
        dynamic_cast<Model*>(spModel.get())->SetName("Light");
        _renderer.AddRenderable(spModel.get());
        _renderObjects.push_back(std::move(spModel));
    }

    {
        auto spModel = std::make_unique<Model>();
        auto spRenderUnit = std::make_unique<RenderUnit>();
        spRenderUnit->spMesh = algorithm::PrimitiveToMesh(Plane(2.f, 2.f));
        spModel->AddRenderUnit(std::move(spRenderUnit));
        const Eigen::Affine3f rotation(Eigen::AngleAxis<float>(-std::numbers::pi_v<float> * 0.5f, Eigen::Vector3f::UnitX()));
        Eigen::Matrix4f rot = Eigen::Matrix4f::Zero();
        rot.block(0, 0, 3, 3) = rotation.rotation();
        rot(3,3) = 1.f;
        auto& renderUnits = spModel->GetRenderUnits();
        for (auto& renderUnit : renderUnits) {
            auto spShader = std::make_unique<GridFloorShader>();
            spShader->SetCamera(_spCamera);
            spShader->SetName("GridShader");
            renderUnit->pShader = spModel->AddShader(std::move(spShader));
            renderUnit->spOpenGLData = std::make_unique<OpenGL3DDataObject>();
            renderUnit->InitializeRenderData();
        }
        dynamic_cast<Model*>(spModel.get())->CreateBoundingVolumes(Primitives::Types::Cuboid);
        dynamic_cast<Model*>(spModel.get())->SetName("FloorGrid");
        _renderer.AddRenderable(spModel.get());
        _renderObjects.push_back(std::move(spModel));
    }

    render();
}

// RenderLoop
void Viewer3D::render() {

    uint32_t fps = 60;
    CHECK_GL_ERROR_(glClearColor(_backgroundColor.x() * _backgroundColor.w(),
                                 _backgroundColor.y() * _backgroundColor.w(),
                                 _backgroundColor.z() * _backgroundColor.w(),
                                 _backgroundColor.w()));

    CHECK_GL_ERROR_(glEnable(GL_DEPTH_TEST));
    CHECK_GL_ERROR_(glDepthFunc(GL_LESS))
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    while (!glfwWindowShouldClose(_spWindow->GetGLFWWindow())) {
        auto t1 = std::chrono::high_resolution_clock::now();
        CHECK_GL_ERROR_(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))
        _spWindow->HandleInputEvent();

        // UpdatePositions
        _spCamera->UpdateCameraPosition(_spWindow->GetCursorPostionDelta(), 1. / static_cast<double>(fps));
        // Draw
        for (auto& spRenderAble : _renderObjects) {
            auto spObject = dynamic_cast<Model*>(spRenderAble.get());
            if (spObject != nullptr) {
                spObject->CheckCollision(_spCamera.get());
            }
        }
        _renderer.Render();
        glfwSwapBuffers(_spWindow->GetGLFWWindow());

        glfwPollEvents();  // should be invoked after glfwSwapBuffers
        auto t2 = std::chrono::high_resolution_clock::now();
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        fps = static_cast<uint32_t>(1000L / std::max(time_elapsed, 1L));
    }
}
