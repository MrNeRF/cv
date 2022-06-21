#ifndef SHADER_H
#define SHADER_H

#include "File.h"
#include "Logger.h"
#include <Eigen/Dense>
#include <GL/glew.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

class Shader
{
public:
    enum class ShaderType
    {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };

public:
    explicit Shader(const std::string& name)
        : shaderName(name){};

    void         InitShaders(const File& vertexShaderRaw, const File& fragmentShaderRaw);
    void         UseShader(void) { CHECK_GL_ERROR_(glUseProgram(shaderProgramID)); }
    unsigned int GetShaderProgramID(void) { return shaderProgramID; };

    template<typename T>
    void SetValue(const std::string& name, T value);
    void SetQuat(const std::string& name, const Eigen::Quaternionf& v)
    {
        CHECK_GL_ERROR_(glUniform4f(glGetUniformLocation(shaderProgramID, name.c_str()), v.x(), v.y(), v.z(), v.w()));
    }
    void SetVector(const std::string& name, const Eigen::Vector3f& v)
    {
        CHECK_GL_ERROR_(glUniform3f(glGetUniformLocation(shaderProgramID, name.c_str()), v.x(), v.y(), v.z()));
    }
    void SetVector(const std::string& name, const Eigen::Vector4f& v)
    {
        CHECK_GL_ERROR_(glUniform4f(glGetUniformLocation(shaderProgramID, name.c_str()), v.x(), v.y(), v.z(), v.w()));
    }
    void SetTransformationMatrix(const std::string& name, const Eigen::Matrix4f& mat)
    {
        CHECK_GL_ERROR_(glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, mat.data()));
    }
    void SetTransformationMatrix(const std::string& name, const Eigen::Matrix3f& mat)
    {
        CHECK_GL_ERROR_(glUniformMatrix3fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, mat.data()));
    }

public:
    const std::string shaderName;

private:
    unsigned int createShader(const std::string& shaderProgramCode, const Shader::ShaderType& shaderType);
    void         createShaderProgram(const std::vector<unsigned int>& IDs);
    void         checkCompileErrors(unsigned int shaderID, const Shader::ShaderType& shaderType);

private:
    unsigned int                      shaderProgramID;
    std::map<ShaderType, std::string> shaderTypeMapping = {{ShaderType::VERTEX, "VERTEX"},
                                                           {ShaderType::FRAGMENT, "FRAGMENT"},
                                                           {ShaderType::PROGRAM, "PROGRAM"}};
};

template<typename T>
void Shader::SetValue(const std::string& name, T value)
{
    if (std::is_floating_point<T>::value)
    {
        CHECK_GL_ERROR_(glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value));
    }
    else
    {
        CHECK_GL_ERROR_(glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value));
    }
}

#endif
