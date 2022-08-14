//
// Created by paja on 8/14/22.
//

#ifndef CV_RENDERUNIT_H
#define CV_RENDERUNIT_H

#include <memory>
#include <Material.h>
#include <Mesh.h>
#include "Shader.h"

struct RenderUnit {
    std::unique_ptr<Mesh> spMesh;
    const Shader* pShader = nullptr;
    const Material* pMaterial = nullptr;

};

#endif  // CV_RENDERUNIT_H
