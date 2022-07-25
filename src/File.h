#ifndef FILE_OBJECT_H
#define FILE_OBJECT_H

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class File {
   public:
    enum class FileType {
        Model,
        Shader,
        Video,
        Image
    };

   public:
    explicit File(const std::string& fileName, const FileType fileType);
    std::string GetContents() const;
    const std::filesystem::path GetFilePath() const;
    std::vector<std::string> GetDirectoryContents() const;
    std::string GetFileName() const;
    std::string GetFileEnding() const;

   private:
    const std::filesystem::path _inputPath;
};

#endif
