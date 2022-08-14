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
    : _inputPath{std::filesystem::current_path().parent_path().string() + "/" + fileTypeFolderMapping(fileType) + "/" + fileName} {

    if (exists(_inputPath)) {
        return;
    } else {
        for (const std::filesystem::directory_entry& dirEntry : std::filesystem::recursive_directory_iterator(_inputPath.parent_path())) {
            std::string fn = dirEntry.path().filename().string();
            if (fn ==_inputPath.filename().string()) {
                _inputPath  = dirEntry.path();
                break;
            }
        }
    }
}

std::string File::GetContents() const {
    std::ifstream fileReader;
    fileReader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        fileReader.open(_inputPath.c_str());
        std::stringstream ss;
        ss << fileReader.rdbuf();
        fileReader.close();
        return ss.str();
    } catch (const std::ifstream::failure& e) {
        std::cerr << fmt::format("ERROR: File {} not successfully read!\n", _inputPath.string());
        return "";
    }
}

const std::filesystem::path File::GetFilePath() const {
    return _inputPath;
}

std::vector<std::string> File::GetDirectoryContents() const {
    std::vector<std::string> filenames;
    for (const auto& rFile : std::filesystem::directory_iterator(_inputPath)) {
        std::string filename = rFile.path().stem();
        if (std::find(filenames.begin(), filenames.end(), filename) == filenames.end()) {
            filenames.push_back(filename);
        }
    }
    return filenames;
}

std::string File::GetFileEnding() const {
    return _inputPath.extension().string();
}
std::string File::GetFileName() const {
    return _inputPath.stem();
}
