#pragma once

#include <string>
#include <unordered_map>
#include <glm/vec4.hpp>

typedef unsigned int GLenum;

class Shader
{
public:
    Shader(const std::string& a_Filepath);
    Shader(const std::string& aName, const std::string& aVertexSource, const std::string& aFragmentSource);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniformLocation(std::string& aName, glm::vec4 aLocation);
    int GetUniformLocation(const std::string& aName);
    void Compile(const std::unordered_map<GLenum, std::string>& aShaderSources);

    std::string ReadFile(const std::string& aFilepath);

    std::unordered_map<GLenum, std::string> PreProcess(const std::string& aSource);

private:
    unsigned int myRendererID;
    std::string myFilepath;
    std::string myName;
};