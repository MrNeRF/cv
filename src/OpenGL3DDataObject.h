#ifndef OPENLGL_3D_DATA_OBJECT_H
#define OPENLGL_3D_DATA_OBJECT_H

#include <GL/glew.h>
#include <limits>
#include <vector>
#include "Eigen/Dense"

struct Mesh;
struct Texture;
struct RenderUnit;

class OpenGL3DDataObject {
   public:
    OpenGL3DDataObject();
    ~OpenGL3DDataObject(void);
    void InitializeBuffer(const RenderUnit *pRenderUnit);
    void DrawObject(GLenum mode) const;
    void UpdateTextureBuffer(const Texture* pTexture);
    private:
    void initializeVertexData(const Mesh& mesh);
    void initializeTextureData(const Texture* pTexture);

   private:
    // Vertex Array Object
    unsigned int _VAO = 0;
    unsigned int _VBO = 0;
    unsigned int _Texture1 = std::numeric_limits<unsigned int>::max();
    uint32_t _vertexRenderCount = 0;
};

#endif
