#include "Shader.h"
#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <array>

static GLenum ShaderTypeFromString(const std::string& type)
{
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel")
        return GL_FRAGMENT_SHADER;

    return 0;
}

Shader::Shader(const std::string& aFilepath)
    : myFilepath(aFilepath)
    , myRendererID(0)
{
    std::string source = ReadFile(myFilepath);
    auto shaderSources = PreProcess(source);
    Compile(shaderSources);

    auto lastSlash = aFilepath.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = aFilepath.rfind('.');
    auto count = lastDot == std::string::npos ? aFilepath.size() - lastSlash : lastDot - lastSlash;
    myName = aFilepath.substr(lastSlash, count);

    std::cout << "Compiled " << myName << std::endl;
}

Shader::Shader(const std::string& aName, const std::string& aVertexSource, const std::string& aFragmentSource)
    : myName(aName)
{
    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = aVertexSource;
    sources[GL_FRAGMENT_SHADER] = aFragmentSource;
    Compile(sources);
}

Shader::~Shader()
{
    glDeleteProgram(myRendererID);
}

void Shader::Bind() const
{
    glUseProgram(myRendererID);
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
    int location = glGetUniformLocation(myRendererID, aName.c_str());

    return location;
}

void Shader::Compile(const std::unordered_map<GLenum, std::string>& aShaderSources)
{
    GLuint program = glCreateProgram();
    if (aShaderSources.size() > 2)
    {
        std::cout << "We only support 2 shaders for now" << std::endl;
    }

    std::array<GLenum, 2> glShaderIDs;
    int glShaderIDIndex = 0;
    for (auto& kv : aShaderSources)
    {
        GLenum type = kv.first;
        const std::string& source = kv.second;

        GLuint shader = glCreateShader(type);

        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);

            if (!infoLog.data())
            {
                std::cout << "Shader error" << std::endl;
            }

            break;
        }

        glAttachShader(program, shader);
        glShaderIDs[glShaderIDIndex++] = shader;
    }

    myRendererID = program;

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);

        for (auto id : glShaderIDs)
            glDeleteShader(id);

        if (!infoLog.data())
        {
            std::cout << "Shader error" << std::endl;
        }

        return;
    }

    for (auto id : glShaderIDs)
    {
        glDetachShader(program, id);
        glDeleteShader(id);
    }
}

std::string Shader::ReadFile(const std::string& aFilepath)
{
    std::string result;
    std::ifstream in(aFilepath, std::ios::in | std::ios::binary);
    if (in)
    {
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        if (size != -1)
        {
            result.resize(size);
            in.seekg(0, std::ios::beg);
            in.read(&result[0], size);
            in.close();
        }
        else
        {
            std::cout << "Could not read from file " << aFilepath << std::endl;
        }
    }
    else
    {
        std::cout << "Could not open file " << aFilepath << std::endl;
    }

    return result;
}

std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& aSource)
{
    std::unordered_map<GLenum, std::string> shaderSources;

    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = aSource.find(typeToken, 0); //Start of shader type declaration line
    while (pos != std::string::npos)
    {
        size_t eol = aSource.find_first_of("\r\n", pos); //End of shader type declaration line
        if (eol == std::string::npos)
        {
            std::cout << "Syntax error" << std::endl;
        }

        size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
        std::string type = aSource.substr(begin, eol - begin);

        if (ShaderTypeFromString(type) == 0)
        {
            std::cout << "Invalid shader type specified" << std::endl;
        }

        size_t nextLinePos = aSource.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line

        if (nextLinePos == std::string::npos)
        {
            std::cout << "Syntax error" << std::endl;
        }
        pos = aSource.find(typeToken, nextLinePos); //Start of next shader type declaration line

        shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? aSource.substr(nextLinePos) : aSource.substr(nextLinePos, pos - nextLinePos);
    }

    return shaderSources;
}
