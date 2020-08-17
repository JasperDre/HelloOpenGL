#include "RenderContext.h"
#include "ShaderLibrary.h"
#include "GLError.h"

static float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
};

RenderContext::RenderContext()
	: myShaderLibrary(nullptr)
	, myVertexArrayID(0)
	, myVertexBufferID(0)
{
	if (!gladLoadGL())
	{
		printf("Failed to load GLAD\n");
		return;
	}

	PrintDebugInfo();
	CompileShaders();
	GenerateVertexArrayObject();
	GenerateVertexBufferObject();
	BindBuffer();

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(ErrorCallback, nullptr);
}

RenderContext::~RenderContext()
{
	glDeleteVertexArrays(1, &myVertexArrayID);
	glDeleteBuffers(1, &myVertexBufferID);
}

void RenderContext::PrintDebugInfo()
{
	printf("OpenGL %s\n", glGetString(GL_VERSION));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void RenderContext::CompileShaders()
{
	myShaderLibrary = new ShaderLibrary();
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void RenderContext::BindBuffer()
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void RenderContext::Render(int aWidth, int aHeight)
{
	glViewport(0, 0, aWidth, aHeight);
	glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myShaderLibrary->BindShaders();

	glBindVertexArray(myVertexArrayID);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}
