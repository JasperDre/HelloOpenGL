#include "RenderContext.h"
#include "Shader.h"

#include <glad/glad.h>

#include <iostream>

RenderContext::RenderContext()
{
	if (!gladLoadGL())
	{
		std::cout << "Error: " << "Failed to load GLAD" << std::endl;
		return;
	}

	PrintDebugInfo();

	Shader shader("../../Triangle/Data/Shaders/FlatColor.glsl");
}

RenderContext::~RenderContext()
{
}

void RenderContext::PrintDebugInfo()
{
	printf("OpenGL %s\n", glGetString(GL_VERSION));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void RenderContext::Render(int aWidth, int aHeight)
{
	glViewport(0, 0, aWidth, aHeight);
	glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
