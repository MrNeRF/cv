#include "OpenGL3DDataObject.h"
#include "Logger.h"
#include "Mesh.h"
#include "Texture.h"

OpenGL3DDataObject::OpenGL3DDataObject() {
    CHECK_GL_ERROR_(glGenVertexArrays(1, &_VAO));
    CHECK_GL_ERROR_(glGenBuffers(1, &_VBO));
}

OpenGL3DDataObject::~OpenGL3DDataObject(void) {
    // Delete Array Buffers.
    CHECK_GL_ERROR_(glDeleteVertexArrays(1, &_VAO))
    // Delete Indexbuffer
    CHECK_GL_ERROR_(glDeleteBuffers(1, &_VBO))
}

void OpenGL3DDataObject::InitializeBuffer(Mesh& mesh) {
    _vertexRenderCount = mesh.vertices.size();
    uint32_t dataCount = 3;
    dataCount += 2;
    dataCount += 3;
    dataCount += 4;
    //
    //    const float* pData  = mesh.faces.transpose().data();
    //    for(uint32_t i = 0; i < 24 ; i += 8)
    //    {
    //        std::cout << "Vector: " << pData[i + 0] << ", " << pData[i + 1] << ", " << pData[i + 2] << std::endl;
    //        std::cout << "Vector: " << mesh.faces(i / 8, 0) << ", " << mesh.faces(i / 8, 1) << ", " << mesh.faces(i / 8, 2) << std::endl;
    //        std::cout << "UV: " << pData[i + 3] << ", " << pData[i + 4] << std::endl;
    //        std::cout << "UV: " << mesh.faces(i / 8, 3) << ", " << mesh.faces(i / 8, 4) << std::endl;
    //        std::cout << "Normal: " << pData[i + 5] << ", " << pData[i + 6] << ", " << pData[i + 7] << std::endl;
    //        std::cout << "Normal: " << mesh.faces(i / 8, 5) << ", " << mesh.faces(i / 8, 3) << ", " << mesh.faces(i / 8, 7) << std::endl;
    //    }
    CHECK_GL_ERROR_(glBindVertexArray(_VAO))
    CHECK_GL_ERROR_(glBindBuffer(GL_ARRAY_BUFFER, _VBO))
    CHECK_GL_ERROR_(glBufferData(GL_ARRAY_BUFFER, _vertexRenderCount * sizeof(float) * dataCount, mesh.vertices.data(), GL_STATIC_DRAW))


    // vertices
    CHECK_GL_ERROR_(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, dataCount * sizeof(float), (void*)offsetof(Vertex, position)))
    CHECK_GL_ERROR_(glEnableVertexAttribArray(0))

    if (mesh.bHasUVs) {
        CHECK_GL_ERROR_(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, dataCount * sizeof(float), (void*)offsetof(Vertex,uv)))
        CHECK_GL_ERROR_(glEnableVertexAttribArray(1))
    }

    if (mesh.bHasNormals) {
        CHECK_GL_ERROR_(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, dataCount * sizeof(float), (void*)offsetof(Vertex, normal)))
        CHECK_GL_ERROR_(glEnableVertexAttribArray(2))
    }

    if (mesh.bHasColor) {
        CHECK_GL_ERROR_(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, dataCount * sizeof(float), (void*)offsetof(Vertex, color)))
        CHECK_GL_ERROR_(glEnableVertexAttribArray(3))
    }

    // unbind
    CHECK_GL_ERROR_(glBindVertexArray(0));
}

void OpenGL3DDataObject::InitializeTextureBuffer(Texture* pTexture) {
    if (pTexture == nullptr) {
        return;
    }
    glGenTextures(1, &_Texture1);
    glBindTexture(GL_TEXTURE_2D, _Texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Here is a hack for a greyscale picture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pTexture->_width, pTexture->_height, 0, GL_RGB, GL_UNSIGNED_BYTE, pTexture->_pData);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void OpenGL3DDataObject::UpdateTextureBuffer(const Texture* pTexture) {
    if (pTexture == nullptr) {
        return;
    }
    if (_Texture1 != std::numeric_limits<unsigned int>::max()) {
        glBindTexture(GL_TEXTURE_2D, _Texture1);
    } else {
        glGenTextures(1, &_Texture1);
    }
    // Here is a hack for a greyscale picture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pTexture->_width, pTexture->_height, 0, GL_RED, GL_UNSIGNED_BYTE, pTexture->_pData);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void OpenGL3DDataObject::DrawObject(GLenum mode) const {
    glActiveTexture(GL_TEXTURE0);
    if (_Texture1 != std::numeric_limits<unsigned int>::max()) {
        glBindTexture(GL_TEXTURE_2D, _Texture1);
    }
    CHECK_GL_ERROR_(glBindVertexArray(_VAO))
    CHECK_GL_ERROR_(glDrawArrays(mode, 0, _vertexRenderCount))
    CHECK_GL_ERROR_(glBindVertexArray(0))
}
