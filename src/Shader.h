#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <Eigen/Dense>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include "File.h"
#include "Logger.h"

class IRenderable;

class Shader {
   public:
    enum class ShaderType {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };

   public:
    explicit Shader(const std::string& name)
        : _shaderName(name){};

    virtual void ActivateShader(const IRenderable* pRenderObject) = 0;
    unsigned int GetShaderProgramID(void) { return _shaderProgramID; };

    template <typename T>
    void SetValue(const std::string& name, T value);
    void SetQuat(const std::string& name, const Eigen::Quaternionf& v) {
        CHECK_GL_ERROR_(glUniform4f(glGetUniformLocation(_shaderProgramID, name.c_str()), v.x(), v.y(), v.z(), v.w()));
    }
    void SetVector(const std::string& name, const Eigen::Vector3f& v) {
        CHECK_GL_ERROR_(glUniform3f(glGetUniformLocation(_shaderProgramID, name.c_str()), v.x(), v.y(), v.z()));
    }
    void SetVector(const std::string& name, const Eigen::Vector4f& v) {
        CHECK_GL_ERROR_(glUniform4f(glGetUniformLocation(_shaderProgramID, name.c_str()), v.x(), v.y(), v.z(), v.w()));
    }
    void SetTransformationMatrix(const std::string& name, const Eigen::Matrix4f& mat) {
        CHECK_GL_ERROR_(glUniformMatrix4fv(glGetUniformLocation(_shaderProgramID, name.c_str()), 1, GL_FALSE, mat.data()));
    }
    void SetTransformationMatrix(const std::string& name, const Eigen::Matrix3f& mat) {
        CHECK_GL_ERROR_(glUniformMatrix3fv(glGetUniformLocation(_shaderProgramID, name.c_str()), 1, GL_FALSE, mat.data()));
    }

   protected:
    void initShaders(const File& vertexShaderRaw, const File& fragmentShaderRaw);
    void activateShader() { CHECK_GL_ERROR_(glUseProgram(_shaderProgramID)); }

   private:
    unsigned int createShader(const std::string& shaderProgramCode, const Shader::ShaderType& shaderType);
    void createShaderProgram(const std::vector<unsigned int>& IDs);
    void checkCompileErrors(unsigned int shaderID, const Shader::ShaderType& shaderType);

   private:
    const std::string _shaderName;
    unsigned int _shaderProgramID;
    std::map<ShaderType, std::string> _shaderTypeMapping = {{ShaderType::VERTEX, "VERTEX"},
                                                            {ShaderType::FRAGMENT, "FRAGMENT"},
                                                            {ShaderType::PROGRAM, "PROGRAM"}};
};

template <typename T>
void Shader::SetValue(const std::string& name, T value) {
    if (std::is_floating_point<T>::value) {
        CHECK_GL_ERROR_(glUniform1f(glGetUniformLocation(_shaderProgramID, name.c_str()), value));
    } else {
        CHECK_GL_ERROR_(glUniform1i(glGetUniformLocation(_shaderProgramID, name.c_str()), value));
    }
}

#endif
