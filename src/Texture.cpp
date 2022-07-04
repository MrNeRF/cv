#include "Texture.h"
#include <Eigen/Dense>
#include "File.h"
#include "Image.h"
#include "Logger.h"
#include "stb_image.h"

Texture::Texture(const File& file)
    : _name(file.GetFileName() + "." + file.GetFileEnding()), _path(file.GetFilePath()) {
    stbi_set_flip_vertically_on_load(true);
    _pData = stbi_load(_path.c_str(), &_width, &_height, &_nrChannels, 0);
    ASSERT(_pData != nullptr);
    if (_pData == nullptr) {
        Logger::GetInstance().GetLogger().error("Texture {} loading failed", _name);
    } else {
        Logger::GetInstance().GetLogger().info("Texture {} successfully loaded.", _name);
    }
}

Texture::Texture(const Image& image)
    : _name(image.GetName()),
      _width(image.GetWidth()),
      _height(image.GetHeight()),
      _nrChannels(1) {

    _pData = new unsigned char[_width * _height]();
    const auto& imageData = image.GetImageData();

    for (int i = 0; i < _height; ++i) {
        for (int j = 0; j < _width; ++j) {
            _pData[i * _width + j] = (char)imageData(i, j);
        }
    }
}

Texture::~Texture() {
    stbi_image_free(_pData);
}