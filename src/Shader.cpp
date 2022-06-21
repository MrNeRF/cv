#include "Shader.h"
#include "Logger.h"
#include <GL/glew.h>

void Shader::InitShaders(const File& vertexShaderRaw, const File& fragmentShaderRaw)
{
    std::vector<unsigned int> shaderIDs;

    // Vertex Shader Shader
    const unsigned int vertexShaderID = createShader(vertexShaderRaw.GetContents(), ShaderType::VERTEX);
    shaderIDs.push_back(vertexShaderID);
    auto& rLogger = Logger::GetInstance().GetLogger();
    rLogger.info("Vertex Shader {} with ID {} created", vertexShaderRaw.GetFilePath().stem().string(), vertexShaderID);

    // Fragment Shader
    const unsigned int fragmentShaderID = createShader(fragmentShaderRaw.GetContents(), ShaderType::FRAGMENT);
    shaderIDs.push_back(fragmentShaderID);
    rLogger.info("Fragment Shader {} with ID {} created", fragmentShaderRaw.GetFilePath().stem().string(), fragmentShaderID);

    // Shader Program
    createShaderProgram(shaderIDs);
    rLogger.info("Shader Program created");
}

unsigned int Shader::createShader(const std::string& shaderProgramCode, const Shader::ShaderType& shaderType)
{
    unsigned int shaderID;
    switch (shaderType)
    {
        case ShaderType::FRAGMENT:
            CHECK_GL_ERROR_WITH_OUTPUT_(shaderID, glCreateShader(GL_FRAGMENT_SHADER));
            break;
        case ShaderType::VERTEX:
            CHECK_GL_ERROR_WITH_OUTPUT_(shaderID, glCreateShader(GL_VERTEX_SHADER));
            break;
        case ShaderType::PROGRAM:
            break;
        default:
            break;
    }
    const GLchar* code = (const GLchar*)shaderProgramCode.c_str();
    CHECK_GL_ERROR_(glShaderSource(shaderID, 1, &code, NULL));
    CHECK_GL_ERROR_(glCompileShader(shaderID));
    checkCompileErrors(shaderID, shaderType);
    return shaderID;
}

void Shader::createShaderProgram(const std::vector<unsigned int>& IDs)
{
    CHECK_GL_ERROR_WITH_OUTPUT_(shaderProgramID, glCreateProgram())
    for (const auto& id: IDs)
    {
        CHECK_GL_ERROR_(glAttachShader(shaderProgramID, id));
    }
    CHECK_GL_ERROR_(glLinkProgram(shaderProgramID));
    checkCompileErrors(shaderProgramID, ShaderType::PROGRAM);
    for (const auto& id: IDs)
    {
        CHECK_GL_ERROR_(glDeleteShader(id));
    }
}

void Shader::checkCompileErrors(unsigned int shaderID, const Shader::ShaderType& shaderType)
{
    int  success;
    char infoLog[GL_INFO_LOG_LENGTH];

    // Lambda to print compile Error
    auto printCompileError = [this](const Shader::ShaderType& type, char* infoLog)
    {
        auto& rLogger = Logger::GetInstance().GetLogger();
        rLogger.error("{}: Shader Link Error of Type: {}, {}", shaderName, shaderTypeMapping[type], infoLog);
    };

    auto& rLogger = Logger::GetInstance().GetLogger();
    switch (shaderType)
    {
        case ShaderType::FRAGMENT:
        case ShaderType::VERTEX:
            CHECK_GL_ERROR_(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success))
            if (!success)
            {
                glGetShaderInfoLog(shaderID, GL_INFO_LOG_LENGTH, NULL, &infoLog[0]);
                printCompileError(shaderType, infoLog);
            }
            else
            {
                rLogger.info("{} successfully compiled!", shaderTypeMapping[shaderType]);
            }
            break;
        case ShaderType::PROGRAM:
            glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shaderID, GL_INFO_LOG_LENGTH, NULL, &infoLog[0]);
                printCompileError(shaderType, infoLog);
            }
            else
            {
                rLogger.info("{} successfully compiled!", shaderTypeMapping[shaderType]);
            }
            break;
        default:
            break;
    };
}
