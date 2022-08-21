#ifndef PRIMITIVES_3D_H
#define PRIMITIVES_3D_H

#include <memory>
#include "Mesh.h"
#include "Primitives.h"

namespace algorithm {
    std::unique_ptr<Mesh> PrimitiveToMesh(const Primitives& rPrimitive);
}

#endif