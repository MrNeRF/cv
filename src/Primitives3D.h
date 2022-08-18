#ifndef PRIMITIVES_3D_H
#define PRIMITIVES_3D_H

#include "Mesh.h"
#include <memory>

class Primitives3D
{
public:
    static std::unique_ptr<Mesh> CreateCube(const int32_t width, const int32_t height, const int32_t depth);
};

#endif
