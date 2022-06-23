#ifndef OBJECT_FILE_PARSER
#define OBJECT_FILE_PARSER

#include <Eigen/Dense>
#include <memory>
#include <string>
#include <vector>
#include "File.h"

struct Mesh;
struct Texture;

class ObjFileParser {
   public:
    static std::unique_ptr<Mesh> GetMesh(const File& rRawData);
    static std::unique_ptr<Texture> GetTexture(const File& rRawData);
};

#endif
