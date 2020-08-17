#include "Shader.h"
#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Shader::Shader(const std::string& aPath, ShaderType aType)
{
    myPath = aPath;
    myName = GetNameFromPath(aPath);

    std::string shaderSource = ReadFile(aPath);
    myType = GetShaderType(aType);

    Compile(shaderSource);
}

Shader::~Shader()
{
    glDeleteProgram(myProgramID);
}

std::string Shader::ReadFile(const std::string& aPath)
{
    std::string sourceResult;
    std::ifstream sourceStream(aPath, std::ios::in);

    if (!sourceStream.is_open())
    {
        std::cout << "Failed to read file " << aPath << std::endl;
        return "";
    }

    std::stringstream sourceStringStream;
    sourceStringStream << sourceStream.rdbuf();
    sourceResult = sourceStringStream.str();
    sourceStream.close();

    return sourceResult;
}

void Shader::Compile(const std::string& aShaderSource)
{
    GLuint myProgramID = glCreateProgram();
    GLuint shaderID = glCreateShader(myType);

    std::cout << "Compiling " << myName << std::endl;

    const GLchar* sourceCStr = aShaderSource.c_str();
    glShaderSource(shaderID, 1, &sourceCStr, 0);
    glCompileShader(shaderID);

    GLint isCompiled = GL_FALSE;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxInfoLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxInfoLength);

        if (maxInfoLength > 0)
        {
            std::vector<GLchar> infoLog(maxInfoLength);
            glGetShaderInfoLog(shaderID, maxInfoLength, &maxInfoLength, &infoLog[0]);
            std::cout << &infoLog[0] << std::endl;
        }
    }

    std::cout << "Linking program to " << myName << std::endl;

    glAttachShader(myProgramID, shaderID);
    glLinkProgram(myProgramID);

    GLint isLinked = GL_FALSE;
    glGetShaderiv(shaderID, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxInfoLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxInfoLength);

        if (maxInfoLength > 0)
        {
            std::vector<GLchar> infoLog(maxInfoLength);
            glGetShaderInfoLog(shaderID, maxInfoLength, &maxInfoLength, &infoLog[0]);
            std::cout << &infoLog[0] << std::endl;
        }
    }

    glDetachShader(myProgramID, shaderID);
    glDeleteShader(shaderID);

    std::cout << "Compiled " << myName << std::endl;
}

void Shader::Bind() const
{
    glUseProgram(myProgramID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniformLocation(std::string& aName, glm::vec4 aLocation)
{
    glUniform4f(GetUniformLocation(aName), aLocation.x, aLocation.y, aLocation.z, aLocation.w);
}

int Shader::GetUniformLocation(const std::string& aName)
{
    int location = glGetUniformLocation(myProgramID, aName.c_str());

    return location;
}

std::string Shader::GetNameFromPath(const std::string& aPath)
{
    size_t lastSlash = aPath.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    size_t lastDot = aPath.rfind('.');
    size_t count = lastDot == std::string::npos ? aPath.size() - lastSlash : lastDot - lastSlash;
    return aPath.substr(lastSlash, count);
}

unsigned int Shader::GetShaderType(ShaderType aType)
{
    if (aType == ShaderType::Vertex)
    {
        return GL_VERTEX_SHADER;
    }
    else if (aType == ShaderType::Fragment)
    {
        return GL_FRAGMENT_SHADER;
    }

    return 0;
}
