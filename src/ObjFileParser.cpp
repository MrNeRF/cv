#include "ObjFileParser.h"
#include <functional>
#include <unordered_map>
#include "Logger.h"
#include "Macros.h"
#include "Mesh.h"
#include "Texture.h"

static void tokenize(std::string& line, char delim, std::vector<std::string>& tokens) {
    auto start = find(cbegin(line), cend(line), delim);
    tokens.push_back(std::string(cbegin(line), start));

    while (start != cend(line)) {
        const auto finish = find(++start, cend(line), delim);

        tokens.push_back(std::string(start, finish));
        start = finish;
    }
}

std::unique_ptr<Mesh> ObjFileParser::GetMesh(const File& rRawData) {
    const std::string buffer = rRawData.GetContents();
    std::unique_ptr<Mesh> spMesh = std::make_unique<Mesh>();
    std::istringstream iss(buffer);

    std::vector<Eigen::Vector3f> vertices;
    std::vector<Eigen::Vector2f> uvs;
    std::vector<Eigen::Vector3f> normals;
    std::vector<uint32_t> indices;
    uint32_t uiVertexCount, uiNormalCount, uiUVCount;
    uiVertexCount = uiNormalCount = uiUVCount = 0;
    for (std::string line; std::getline(iss, line);) {
        std::vector<std::string> tokens;
        tokenize(line, ' ', tokens);

        if (tokens.at(0).compare("v") == 0 && tokens.size() == 4) {  // Vertex
            vertices.emplace_back(std::stof(tokens[1]), std::stof(tokens.at(2)), std::stof(tokens.at(3)));
            ++uiVertexCount;
        } else if (tokens.at(0).compare("vt") == 0 && tokens.size() == 3) {  // Texture
            uvs.emplace_back(std::stof(tokens[1]), std::stof(tokens.at(2)));
            ++uiUVCount;
        } else if (tokens.at(0).compare("vn") == 0 && tokens.size() == 4) {  // normals;
            // normalData
            normals.emplace_back(std::stof(tokens[1]), std::stof(tokens.at(2)), std::stof(tokens.at(3)));
            ++uiNormalCount;
        } else if (tokens.at(0).compare("f") == 0 && tokens.size() == 4) {  // faces
            // start of face index enumeration.
            std::vector<std::string> subTokens;
            subTokens.reserve(3);
            for (int i = 0; i < 3; ++i) {
                tokenize(tokens.at(i + 1), '/', subTokens);
                // Indices start at 1 that is why we have to subtract 1
                // format is: vertex, uv, normal
                indices.emplace_back(std::stoi(subTokens[0]) - 1);
                indices.emplace_back(std::stoi(subTokens[1]) - 1);
                indices.emplace_back(std::stoi(subTokens[2]) - 1);
                subTokens.clear();
            }
        } else if (tokens.at(0).compare("f") == 0 && tokens.size() == 5) {  // faces
            // Quads  => TODO
            ASSERT(0);
        }
    }

    spMesh->bHasUVs = true;
    spMesh->bHasNormals = true;
    spMesh->faces = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajorBit>(indices.size() / 3, 3 + 2 + 3);
    auto& faces = spMesh->faces;
    for (size_t i = 0, j = 0; i < indices.size(); i += 3, ++j) {
        const Eigen::Vector3f& v = vertices[indices[i]];
        const Eigen::Vector2f& uv = uvs[indices[i + 1]];
        const Eigen::Vector3f& n = normals[indices[i + 2]];
        faces(j, 0) = v.x();
        faces(j, 1) = v.y();
        faces(j, 2) = v.z();
        faces(j, 3) = uv.x();
        faces(j, 4) = uv.y();
        faces(j, 5) = n.x();
        faces(j, 6) = n.y();
        faces(j, 7) = n.z();
    }

    Logger::GetInstance().GetLogger().info("{}: #Vertices: {}, #UVs: {}, #Normals: {}, #Faces: {}",
                                           rRawData.GetFilePath().filename().string(),
                                           uiVertexCount,
                                           uiUVCount,
                                           uiNormalCount,
                                           spMesh->faces.rows() / (3 * 9));

    return spMesh;
}

std::unique_ptr<Texture> ObjFileParser::GetTexture(const File& rRawData) {
    std::string filename;

    {
        const std::string buffer = rRawData.GetContents();
        std::istringstream iss(buffer);
        for (std::string line; std::getline(iss, line);) {
            std::vector<std::string> tokens;
            tokenize(line, ' ', tokens);

            if (tokens.at(0).compare("mtllib") == 0 && tokens.size() == 2) {
                filename = tokens.at(1);
                break;
            }
        }
    }

    if (!filename.empty()) {
        auto materialFile = File(filename, File::FileType::Model);
        const std::string buffer = materialFile.GetContents();
        std::istringstream iss(buffer);
        for (std::string line; std::getline(iss, line);) {
            std::vector<std::string> tokens;
            tokenize(line, ' ', tokens);

            if (tokens.at(0).compare("map_Kd") == 0 && tokens.size() == 2) {
                filename = tokens.at(1);
                return std::make_unique<Texture>(File(filename, File::FileType::Model));
            }
        }
    }

    return nullptr;
}
