#ifndef OBJECT_FILE_PARSER
#define OBJECT_FILE_PARSER

#include <Eigen/Dense>
#include <memory>
#include <string>
#include <vector>
#include "File.h"

class Model;
struct Texture;

class ObjFileParser {
   public:
    static std::unique_ptr<Model> ImportModel(const File& rRawData);
};

#endif
