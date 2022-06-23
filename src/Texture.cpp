#include "Texture.h"
#include "Logger.h"
#include "stb_image.h"
#include "File.h"

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

Texture::~Texture() {
    stbi_image_free(_pData);
}