#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <memory>
#include <string>
#include "Mesh.h"
#include "Texture.h"

struct Mesh;
struct Texture;
class Shader;

class IRenderable {
   public:
    virtual ~IRenderable() = default;

    virtual void SetShader(std::unique_ptr<Shader> spShader) = 0;
    virtual void Render() = 0;
    virtual Mesh* GetMesh() = 0;
    virtual Texture* GetTexture() = 0;
    virtual const std::string& GetName() const = 0;
};
#endif
