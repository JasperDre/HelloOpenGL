#include "RenderContext.h"
#include "Shader.h"

#include <glad/glad.h>

#include <iostream>

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

RenderContext::RenderContext()
	: myVertexShader(nullptr)
	, myFragmentShader(nullptr)
	, myVertexArrayID(0)
	, myVertexBufferID(0)
{
	if (!gladLoadGL())
	{
		std::cout << "Error: " << "Failed to load GLAD" << std::endl;
		return;
	}

	PrintDebugInfo();
	GenerateVertexArrayObject();
	GenerateVertexBufferObject();
	CompileShaders();
}

RenderContext::~RenderContext()
{
}

void RenderContext::PrintDebugInfo()
{
	printf("OpenGL %s\n", glGetString(GL_VERSION));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void RenderContext::CompileShaders()
{
	myVertexShader =  new Shader("../../Data/Shaders/RedVertexShader.glsl", ShaderType::Vertex);
	myFragmentShader = new Shader("../../Data/Shaders/RedFragmentShader.glsl", ShaderType::Fragment);
}

void RenderContext::GenerateVertexArrayObject()
{
	glGenVertexArrays(1, &myVertexArrayID);
	glBindVertexArray(myVertexArrayID);
}

void RenderContext::GenerateVertexBufferObject()
{
	glGenBuffers(1, &myVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, myVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

void RenderContext::Render(int aWidth, int aHeight)
{
	glViewport(0, 0, aWidth, aHeight);
	glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myVertexShader->Bind();
	myFragmentShader->Bind();

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, myVertexBufferID);
	glVertexAttribPointer(
		0,                  // attribute 0
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
}
