#ifndef CV_IMPORTER_H
#define CV_IMPORTER_H

#include <string>
#include <memory>

class Model;
class IRenderable;
struct Texture;

class Importer {
   public:
    static std::unique_ptr<IRenderable> ImportModel(const std::string& pathToMesh);
};

#endif  // CV_IMPORTER_H
