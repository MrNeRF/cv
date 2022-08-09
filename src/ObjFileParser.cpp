#include "ObjFileParser.h"
#include <functional>
#include <numeric>
#include <unordered_map>
#include "Logger.h"
#include "Macros.h"
#include "Material.h"
#include "Model.h"
#include "Mesh.h"
#include "Texture.h"

enum class ObjFormatSymbols {
    vertexPosition,
    uv,
    normal,
    face,
    smoothShading,
    objectName,
    groupName,
    materialLib,
    useMaterial,
    comment
};

enum class MaterialObjLibSymbols {
    newMaterial,
    specularExponent,
    ambientColor,
    diffuseColor,
    specularColor,
    ke_dontKnow,
    opticalDensity,
    alphaValue,
    textureName,
    comment
};

static std::unordered_map<std::string, ObjFormatSymbols> objSymbolsMapping{{"v", ObjFormatSymbols::vertexPosition},
                                                                           {"vt", ObjFormatSymbols::uv},
                                                                           {"vn", ObjFormatSymbols::normal},
                                                                           {"f", ObjFormatSymbols::face},
                                                                           {"s", ObjFormatSymbols::smoothShading},
                                                                           {"o", ObjFormatSymbols::objectName},
                                                                           {"mtllib", ObjFormatSymbols::materialLib},
                                                                           {"usemtl", ObjFormatSymbols::useMaterial},
                                                                           {"g", ObjFormatSymbols::groupName},
                                                                           {"#", ObjFormatSymbols::comment}};

static std::unordered_map<std::string, MaterialObjLibSymbols> materialLibSymbolMapping{{"newmtl", MaterialObjLibSymbols::newMaterial},
                                                                                       {"Ns", MaterialObjLibSymbols::specularExponent},
                                                                                       {"Ka", MaterialObjLibSymbols::ambientColor},
                                                                                       {"Kd", MaterialObjLibSymbols::diffuseColor},
                                                                                       {"Ks", MaterialObjLibSymbols::specularColor},
                                                                                       {"Ke", MaterialObjLibSymbols::ke_dontKnow},
                                                                                       {"Ni", MaterialObjLibSymbols::opticalDensity},
                                                                                       {"d", MaterialObjLibSymbols::alphaValue},
                                                                                       {"map_Kd", MaterialObjLibSymbols::textureName},
                                                                                       {"#", MaterialObjLibSymbols::comment}};

enum class FaceFormat {
    undefined,  // None set
    v_vt,       // vertex, uv
    v_vn,       // vertex, normal
    v_vt_vn,    // vertex, uv, normal
    v_v_v       // vertex, vertex, vertex
};

struct VertexIndices {
    uint32_t v_index = std::numeric_limits<uint32_t>::max();
    uint32_t vt_index = std::numeric_limits<uint32_t>::max();
    uint32_t vn_index = std::numeric_limits<uint32_t>::max();
};

static void tokenize(const std::string& line, char delim, std::vector<std::string>& tokens) {

    auto start = find(cbegin(line), cend(line), delim);
    tokens.push_back(std::string(cbegin(line), start));

    while (start != cend(line)) {
        const auto finish = find(++start, cend(line), delim);

        tokens.push_back(std::string(start, finish));
        start = finish;
    }
}

static std::unique_ptr<Material> importMaterial(const std::string materialFileName) {
    auto materialFile = File(materialFileName, File::FileType::Model);
    const std::string buffer = materialFile.GetContents();
    std::istringstream iss(buffer);
    std::unique_ptr<Material> spMaterial = std::make_unique<Material>();
    for (std::string line; std::getline(iss, line);) {
        std::vector<std::string> tokens;
        tokenize(line, ' ', tokens);
        const auto symbol = materialLibSymbolMapping[tokens[0]];
        switch (symbol) {
        case MaterialObjLibSymbols::newMaterial:
            break;
        case MaterialObjLibSymbols::specularExponent:
            spMaterial->specularExponent = std::stof(tokens[1]);
            break;
        case MaterialObjLibSymbols::ambientColor:
            spMaterial->ambient = {std::stof(tokens[1]), std::stof(tokens[2]),std::stof(tokens[3])};
            break;
        case MaterialObjLibSymbols::diffuseColor:
            spMaterial->diffuse = {std::stof(tokens[1]), std::stof(tokens[2]),std::stof(tokens[3])};
            break;
        case MaterialObjLibSymbols::specularColor:
            spMaterial->specular = {std::stof(tokens[1]), std::stof(tokens[2]),std::stof(tokens[3])};
            break;
        case MaterialObjLibSymbols::ke_dontKnow:
            break;
        case MaterialObjLibSymbols::opticalDensity:
            break;
        case MaterialObjLibSymbols::alphaValue:
            break;
        case MaterialObjLibSymbols::textureName:
            spMaterial->spTexure = std::make_unique<Texture>(File(tokens.at(1), File::FileType::Model));
            break;
        case MaterialObjLibSymbols::comment:
            break;
        default:
            ASSERT(0)
            // not implemented
        }
    }

    return spMaterial;
}

std::pair<std::vector<VertexIndices>, FaceFormat> extractIndices(const std::vector<std::string>& rTokens) {
    std::vector<uint32_t> indices;
    std::string indexString;
    FaceFormat faceFormat = FaceFormat::undefined;
    std::vector<VertexIndices> vertexIndices;
    for (const auto& rToken : rTokens) {
        if (rToken == "f") {
            continue;
        }
        uint32_t numberOfSlashes = 0;
        indexString = "";
        for (size_t idx = 0; idx < rToken.size(); ++idx) {
            const char currentChar = rToken[idx];
            if (currentChar == '/') {
                const char secondChar = rToken[idx + 1];
                faceFormat = secondChar == '/' ? FaceFormat::v_vn : numberOfSlashes++ == 1 ? FaceFormat::v_vt
                                                                                           : FaceFormat::v_vt_vn;
                // advance one index because of double slash
                idx = secondChar == '/' ? idx + 1 : idx;
                // shift index by -1 (obj starting from 1)
                indices.push_back(std::stoi(indexString) - 1);
                indexString = "";

            } else {
                indexString += currentChar;
            }
        }
        // take last index and shift index by -1 (obj starting from 1)
        indices.push_back(std::stoi(indexString) - 1);
        faceFormat = faceFormat == FaceFormat::v_vn ? FaceFormat::v_vn : numberOfSlashes == 2 ? FaceFormat::v_vt_vn
                                                                                              : FaceFormat::v_vt;

        switch (faceFormat) {
        case FaceFormat::undefined:  // None set
            break;
        case FaceFormat::v_vt:  // vertex, uv
            vertexIndices.push_back(VertexIndices{.v_index = indices[0], .vt_index = indices[1]});
            break;
        case FaceFormat::v_vn:  // vertex, normal
            vertexIndices.push_back(VertexIndices{.v_index = indices[0], .vn_index = indices[1]});
            break;
        case FaceFormat::v_vt_vn:  // vertex, uv, normal
            vertexIndices.push_back(VertexIndices{.v_index = indices[0], .vt_index = indices[1], .vn_index = indices[2]});
            break;
        case FaceFormat::v_v_v:  // vertex, vertex, vertex
            for (size_t idx = 0; idx < 3; ++idx) {
                vertexIndices.push_back(VertexIndices{.v_index = indices[idx]});
            }
            break;
        default:
            ASSERT(0);
        }
        indices.clear();
    }

    return std::make_pair(vertexIndices, faceFormat);
}

std::unique_ptr<Model> ObjFileParser::ImportModel(const File& rRawData) {
    const std::string buffer = rRawData.GetContents();
    std::unique_ptr<Model> spModel = std::make_unique<Model>();
    std::unique_ptr<Mesh> spMesh = std::make_unique<Mesh>();
    std::istringstream iss(buffer);

    std::vector<Eigen::Vector3f> vertices;
    std::vector<Eigen::Vector2f> uvs;
    std::vector<Eigen::Vector3f> normals;

    for (std::string line; std::getline(iss, line);) {
        std::vector<std::string> tokens;
        tokenize(line, ' ', tokens);

        const auto symbol = objSymbolsMapping[tokens[0]];
        switch (symbol) {
        case ObjFormatSymbols::vertexPosition:
            vertices.emplace_back(std::stof(tokens[1]), std::stof(tokens.at(2)), std::stof(tokens.at(3)));
            break;
        case ObjFormatSymbols::uv:
            uvs.emplace_back(std::stof(tokens[1]), std::stof(tokens.at(2)));
            break;
        case ObjFormatSymbols::normal:
            normals.emplace_back(std::stof(tokens[1]), std::stof(tokens.at(2)), std::stof(tokens.at(3)));
            break;
        case ObjFormatSymbols::materialLib:
            spModel->SetMaterial(importMaterial(tokens[1]));
            break;
        case ObjFormatSymbols::useMaterial:
            break;
        case ObjFormatSymbols::comment:
            // Comment. Skip!
            break;
        case ObjFormatSymbols::groupName:
            break;
        case ObjFormatSymbols::objectName:
            spModel->SetName(tokens[0]);
            break;
        case ObjFormatSymbols::smoothShading:
            break;
        case ObjFormatSymbols::face: {
            auto [vertexIndices, fFormat] = extractIndices(tokens);
            if (vertexIndices.size() == 3) {
                for (const auto& rVertexIndex : vertexIndices) {
                    Vertex vertex;
                    vertex.position = vertices[rVertexIndex.v_index];
                    switch (fFormat) {
                    case FaceFormat::undefined:
                        break;
                    case FaceFormat::v_vt:
                        vertex.uv = uvs[rVertexIndex.vt_index];
                        spMesh->bHasUVs = true;
                        break;
                    case FaceFormat::v_vn:
                        vertex.normal = normals[rVertexIndex.vn_index];
                        spMesh->bHasNormals = true;
                        break;
                    case FaceFormat::v_vt_vn:
                        vertex.uv = uvs[rVertexIndex.vt_index];
                        vertex.normal = normals[rVertexIndex.vn_index];
                        spMesh->bHasNormals = true;
                        spMesh->bHasUVs = true;
                        break;
                    case FaceFormat::v_v_v:
                        break;
                    }
                    spMesh->vertices.push_back(vertex);
                }
            } else if (vertexIndices.size() == 4) {
                // split quad into two triangles via indices
                std::array<uint32_t, 6> vertexIndex{0, 1, 3, 1, 2, 3};  // First triangle: 0,1,3; second triangle: 1,2,3
                switch (fFormat) {
                case FaceFormat::undefined:
                    break;
                case FaceFormat::v_vt:
                    for (const uint32_t idx : vertexIndex) {
                        Vertex vertex;
                        vertex.position = vertices[idx];
                        vertex.uv = uvs[idx];
                        spMesh->vertices.push_back(vertex);
                        spMesh->bHasUVs = true;
                    }
                    break;
                case FaceFormat::v_vn:
                    for (const uint32_t idx : vertexIndex) {
                        Vertex vertex;
                        vertex.position = vertices[idx];
                        vertex.normal = normals[idx];
                        spMesh->vertices.push_back(vertex);
                        spMesh->bHasNormals = true;
                    }
                    break;
                case FaceFormat::v_vt_vn:
                    for (const uint32_t idx : vertexIndex) {
                        Vertex vertex;
                        vertex.position = vertices[idx];
                        vertex.uv = uvs[idx];
                        vertex.normal = normals[idx];
                        spMesh->vertices.push_back(vertex);
                        spMesh->bHasNormals = true;
                        spMesh->bHasUVs = true;
                    }
                    break;
                case FaceFormat::v_v_v:
                    for (const uint32_t idx : vertexIndex) {
                        Vertex vertex;
                        vertex.position = vertices[idx];
                        spMesh->vertices.push_back(vertex);
                    }
                    break;
                }
            } else {
                ASSERT(0);
            }
            break;
        }
        default:
            Logger::GetInstance().GetLogger().info("{}: Token {} not implemented",
                                                   rRawData.GetFilePath().filename().string(),
                                                   tokens[0]);
            continue;
            break;
            // skip for now
        }
    }

    Logger::GetInstance().GetLogger().info("{}: #Vertices: {}, #UVs: {}, #Normals: {}, #Faces: {}",
                                           rRawData.GetFilePath().filename().string(),
                                           vertices.size(),
                                           uvs.size(),
                                           normals.size(),
                                           spMesh->vertices.size() / 3);

    spModel->SetMesh(std::move(spMesh));
    return spModel;
}