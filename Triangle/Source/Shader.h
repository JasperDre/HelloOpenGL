#pragma once

#include <string>
#include <glm/vec4.hpp>

enum class ShaderType
{
    Vertex = 0,
    Fragment = 1
};

class Shader
{
public:
    Shader(const std::string& aPath, ShaderType aType);
    ~Shader();

    std::string ReadFile(const std::string& aPath);
    void Compile(const std::string& aShaderSource);

    void Bind() const;
    void Unbind() const;

    void SetUniformLocation(std::string& aName, glm::vec4 aLocation);
    int GetUniformLocation(const std::string& aName);

private:
    std::string GetNameFromPath(const std::string& aPath);
    unsigned int GetShaderType(ShaderType aType);

private:
    unsigned int myProgramID;
    unsigned int myType;
    std::string myPath;
    std::string myName;
};