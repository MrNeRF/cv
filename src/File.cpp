#include "File.h"
#include "fmt/core.h"
#include "fstream"
#include "iostream"
#include <algorithm>

File::File(const std::string& filePath)
    : inputPath{std::filesystem::current_path().parent_path().string() + "/" + filePath}
{
    if (is_empty(inputPath) || !exists(inputPath))
    {
        std::cerr << "Error! No filename provided!\n";
        return;
    }
}
//
std::string File::GetContents() const
{
    std::ifstream fileReader;
    fileReader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        fileReader.open(inputPath.c_str());
        std::stringstream ss;
        ss << fileReader.rdbuf();
        fileReader.close();
        return ss.str();
    }
    catch (const std::ifstream::failure& e)
    {
        std::cerr << fmt::format("ERROR: File {} not successfully read!\n", inputPath.string());
        return "";
    }
}

const std::filesystem::path File::GetFilePath() const
{
    return inputPath;
}

std::vector<std::string> File::GetDirectoryContents() const
{
    std::vector<std::string> filenames;
    for (const auto& rFile: std::filesystem::directory_iterator(inputPath))
    {
        std::string filename = rFile.path().stem();
        if (std::find(filenames.begin(), filenames.end(), filename) == filenames.end())
        {
            filenames.push_back(filename);
        }
    }
    return filenames;
}