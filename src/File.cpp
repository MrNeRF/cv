#include "File.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include "Logger.h"

static std::string fileTypeFolderMapping(File::FileType fileType) {
    switch (fileType) {
    case File::FileType::Shader:
        return "shader";
    case File::FileType::Model:
    case File::FileType::Video:
    case File::FileType::Image:
        return "data";
    default:
        // Logger::GetInstance().GetLogger().error("FileType not implemented!");
        return "";
    }
}

File::File(const std::string& fileName, const FileType fileType)
    : inputPath{std::filesystem::current_path().parent_path().string() + "/" + fileTypeFolderMapping(fileType) + "/" + fileName} {
    if (is_empty(inputPath) || !exists(inputPath)) {
        std::cerr << "Error! No filename provided!\n";
        return;
    }
}
//
std::string File::GetContents() const {
    std::ifstream fileReader;
    fileReader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        fileReader.open(inputPath.c_str());
        std::stringstream ss;
        ss << fileReader.rdbuf();
        fileReader.close();
        return ss.str();
    } catch (const std::ifstream::failure& e) {
        std::cerr << fmt::format("ERROR: File {} not successfully read!\n", inputPath.string());
        return "";
    }
}

const std::filesystem::path File::GetFilePath() const {
    return inputPath;
}

std::vector<std::string> File::GetDirectoryContents() const {
    std::vector<std::string> filenames;
    for (const auto& rFile : std::filesystem::directory_iterator(inputPath)) {
        std::string filename = rFile.path().stem();
        if (std::find(filenames.begin(), filenames.end(), filename) == filenames.end()) {
            filenames.push_back(filename);
        }
    }
    return filenames;
}

std::string File::GetFileEnding() const {
    return inputPath.extension().string();
}
std::string File::GetFileName() const {
    return inputPath.stem();
}
