#ifndef OPENLGL_3D_DATA_OBJECT_H
#define OPENLGL_3D_DATA_OBJECT_H

#include <GL/glew.h>
#include <limits>
#include <vector>
#include "Eigen/Dense"

struct Mesh;
struct Texture;

class OpenGL3DDataObject {
   public:
    OpenGL3DDataObject();
    ~OpenGL3DDataObject(void);
    void InitializeBuffer(Mesh& mesh);
    void InitializeTextureBuffer(Texture* pTexture);
    void UpdateTextureBuffer(const Texture* pTexture);
    void DrawObject(GLenum mode) const;

   private:
    // Vertex Array Object
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_Texture1 = std::numeric_limits<unsigned int>::max();
    uint32_t m_vertexRenderCount = 0;
};

#endif
