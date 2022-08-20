//
// Created by paja on 8/14/22.
//

#ifndef CV_RENDERUNIT_H
#define CV_RENDERUNIT_H

#include <Material.h>
#include <Mesh.h>
#include <memory>
#include "OpenGL3DDataObject.h"
#include "Shader.h"

struct RenderUnit {
    std::unique_ptr<Mesh> spMesh;
    Shader* pShader = nullptr;
    const Material* pMaterial = nullptr;
    std::unique_ptr<OpenGL3DDataObject> spOpenGLData;

    void InitializeRenderData() {
        if (pMaterial != nullptr && pMaterial->spTexure != nullptr) {
            spOpenGLData->InitializeBuffer(spMesh.get(), pMaterial->spTexure.get());
        } else {
            spOpenGLData->InitializeBuffer(spMesh.get(), nullptr);
        }
    }
};

#endif  // CV_RENDERUNIT_H
