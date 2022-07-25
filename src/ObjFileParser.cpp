#include "ObjFileParser.h"
#include <functional>
#include <unordered_map>
#include "Logger.h"
#include "Macros.h"
#include "Mesh.h"
#include "Texture.h"

const static std::unordered_map<std::string, uint8_t> objSymbolsMapping{{"v", 'v'}, {"vt", 't'}, {"vn", 'n'}, {"f", 'f'}, {"s", 's'}, {"o", 'o'}, {"mtllib", 'm'}, {"usemtl", 'u'}, {"g", 'g'}, {"#", '#'}};
enum class FaceFormat {
    undefined,  // None set
    v_vt,       // vertex, uv
    v_vn,       // vertex, normal
    v_vt_vn,    // vertex, uv, normal
    v_v_v       // vertex, vertex, vertex
};

std::pair<uint8_t, uint8_t> findSymbol(const std::string& line) {
    int start = 0;
    int end = 0;
    std::string symbol;
    uint8_t index = 0;
    for (const auto& c : line) {
        if (c != ' ') {
            symbol += c;
            ++index;
        } else {
            ++index;
            break;
        }
    }

    auto mappedSymbol = objSymbolsMapping[symbol];
    return std::make_pair(mappedSymbol, index);
}

std::pair<std::vector<uint32_t>, FAceFormat> extractIndices(const std::string& line, uint8_t index) {
    uint32_t strLen = static_cast<uint8_t>(line.size());
    std::vector<uint32_t> indices;
    std::string indexString;
    uint8_t slashCounter = 0;
    FaceFormat fFormat = FaceFormat::undefined;
    for (uint8_t i = index; i < strLen; ++i) {
        if (line[i] == ' ' || (line[i] == '\n') {
            slashCounter = 0;
            indexString = "";
            continue;
        } else if (line[i] == '\\' && line[i + 1] == '\\') {
            // case double slash
            fFormat = FaceFormat::v_vn;
            indices.push_back(std::stoi(indexString));
            indexString = "" ++i;
        } else if (line[i] == '\\' && line[i + 1] != '\\') {
            // case single slash
            ++slashCounter;
            fFormat = slashCounter == 1 ? FaceFormat::v_vt : FaceFormat::v_vt_vn;
            indices.push_back(std::stoi(indexString));
            indexString = "" ++i;
        } else {
            indexString += line[i];
        }
    }
}

static void tokenize(const std::string& line, char delim, std::vector<std::string>& tokens) {

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

    for (std::string line; std::getline(iss, line);) {
        std::vector<std::string> tokens;
        tokenize(line, ' ', tokens);

        auto [symbol, index] = findSymbol(line);
        switch (symbol) {
        case 'v':
            vertices.emplace_back(std::stof(tokens[1]), std::stof(tokens.at(2)), std::stof(tokens.at(3)));
        case 't':
            uvs.emplace_back(std::stof(tokens[1]), std::stof(tokens.at(2)));
        case 'n':
            normals.emplace_back(std::stof(tokens[1]), std::stof(tokens.at(2)), std::stof(tokens.at(3)));
        case 'f':
            auto [tokenizedIndices, fFormat] = extractIndices(line, index);
            // Here we have to distinguish the cases. It is of utmost importance to distinguish quads, etc.

        default:
            // skip for now
        }
        if (tokens.at(0).compare("v") == 0) {  // Vertex
            vertices.emplace_back(std::stof(tokens[1]), std::stof(tokens.at(2)), std::stof(tokens.at(3)));
        } else if (tokens.at(0).compare("vt") == 0) {  // Texture
            uvs.emplace_back(std::stof(tokens[1]), std::stof(tokens.at(2)));
        } else if (tokens.at(0).compare("vn") == 0) {  // normals;
            normals.emplace_back(std::stof(tokens[1]), std::stof(tokens.at(2)), std::stof(tokens.at(3)));
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
            std::vector<std::string> subTokens;
            subTokens.reserve(4);
            for (int i = 0; i < 4; ++i) {
                tokenize(tokens.at(i + 1), '/', subTokens);
                // Indices start at 1 that is why we have to subtract 1
                // format is: vertex, uv, normal
                // first triangle
                indices.emplace_back(std::stoi(subTokens[0]) - 1);
                indices.emplace_back(std::stoi(subTokens[1]) - 1);
                indices.emplace_back(std::stoi(subTokens[3]) - 1);

                // second triangle
                indices.emplace_back(std::stoi(subTokens[0]) - 1);
                indices.emplace_back(std::stoi(subTokens[1]) - 1);
                indices.emplace_back(std::stoi(subTokens[2]) - 1);
                subTokens.clear();
            }
        } else if (tokens.at(0).compare("f") == 0 && tokens.size() > 5) {  // faces
            // Not implemented
            ASSERT(0)
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
                                           vertices.size(),
                                           uvs.size(),
                                           normals.size(),
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
