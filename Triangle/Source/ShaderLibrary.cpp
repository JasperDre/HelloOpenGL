#include "ShaderLibrary.h"
#include "Shader.h"

#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <string>

ShaderLibrary::ShaderLibrary()
	: myProgramID(0)
{
	unsigned int myProgramID = glCreateProgram();

    myVertexShader = new Shader("../../Data/Shaders/RedVertexShader.glsl", ShaderType::Vertex);
    myFragmentShader = new Shader("../../Data/Shaders/RedFragmentShader.glsl", ShaderType::Fragment);

    AttachShaders(*myVertexShader, *myFragmentShader);
}

ShaderLibrary::~ShaderLibrary()
{
	glDeleteProgram(myProgramID);
}

void ShaderLibrary::AttachShaders(const Shader& aVertexShader, const Shader& aFragmentShader)
{
    printf("Linking program [%i] to %s\n", myProgramID, aVertexShader.GetName().c_str());
    printf("Linking program [%i] to %s\n", myProgramID, aFragmentShader.GetName().c_str());

    const unsigned int vertexShaderID = aVertexShader.GetID();
    const unsigned int fragmentShaderID = aFragmentShader.GetID();
    glAttachShader(myProgramID, vertexShaderID);
    glAttachShader(myProgramID, fragmentShaderID);
    glLinkProgram(myProgramID);

    GLint isLinked = GL_FALSE;
    glGetProgramiv(myProgramID, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxInfoLength = 0;
        glGetProgramiv(myProgramID, GL_INFO_LOG_LENGTH, &maxInfoLength);

        if (maxInfoLength > 0)
        {
            std::vector<GLchar> infoLog(maxInfoLength);
            glGetProgramInfoLog(myProgramID, maxInfoLength, &maxInfoLength, &infoLog[0]);
            printf("%s\n", &infoLog[0]);
        }
    }

    glDetachShader(myProgramID, vertexShaderID);
    glDetachShader(myProgramID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void ShaderLibrary::BindShaders()
{
    glUseProgram(myProgramID);
}
