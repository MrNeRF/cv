#ifndef FILE_OBJECT_H
#define FILE_OBJECT_H

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class File
{
public:
    explicit File(const std::string& filePath);
    std::string                 GetContents() const;
    const std::filesystem::path GetFilePath() const;
    std::vector<std::string>    GetDirectoryContents() const;

private:
    const std::filesystem::path inputPath;
};

#endif
