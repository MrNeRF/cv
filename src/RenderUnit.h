//
// Created by paja on 8/14/22.
//

#ifndef CV_RENDERUNIT_H
#define CV_RENDERUNIT_H

#include <memory>
#include <Material.h>
#include <Mesh.h>
#include "Shader.h"
#include "OpenGL3DDataObject.h"

struct RenderUnit {
    std::unique_ptr<Mesh> spMesh;
    Shader* pShader = nullptr;
    const Material* pMaterial = nullptr;
    std::unique_ptr<OpenGL3DDataObject> spOpenGLData;

    void InitializeRenderData() {
        spOpenGLData->InitializeBuffer(spMesh.get(), pMaterial->spTexure.get());
    }

};

#endif  // CV_RENDERUNIT_H
