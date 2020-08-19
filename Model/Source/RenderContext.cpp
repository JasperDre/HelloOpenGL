#include "RenderContext.h"
#include "ShaderLibrary.h"
#include "GLError.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

RenderContext::RenderContext()
	: myShaderLibrary(nullptr)
	, myCamera(nullptr)
	, myModel(nullptr)
	, myVertexArrayID(0)
	, myVertexBufferID(0)
{
	if (!gladLoadGL())
	{
		printf("Failed to load GLAD\n");
		return;
	}

	PrintDebugInfo();

	myModel = new Model("../../Data/Models/Cube/Cube.obj");
	const std::vector<std::string> textureReferences = myModel->GetTextureReferences();
	for (size_t i = 0; i < textureReferences.size(); i++)
	{
		Texture* texture = new Texture(textureReferences[i]);
	}

	CompileShaders();
	GenerateVertexArrayObject();
	GenerateVertexBufferObject();
	BindBuffer();

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(ErrorCallback, nullptr);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	CheckGLError();

	myCamera = new Camera();
	myCamera->myProjection = glm::perspective(glm::radians(45.0f), 1240.0f / 720.0f, 0.1f, 100.0f);
	myCamera->myView = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
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

	CheckGLError();
}

void RenderContext::GenerateVertexBufferObject()
{
	glGenBuffers(1, &myVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, myVertexBufferID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, myModel->vbos.size() * sizeof(float), &myModel->vbos.at(0), GL_STATIC_DRAW);

	CheckGLError();
}

void RenderContext::BindBuffer()
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CheckGLError();
}

void RenderContext::Render(int aWidth, int aHeight)
{
	myShaderLibrary->BindShaders();

	glm::mat4 mvp = myCamera->myProjection * myCamera->myView * myModel->GetModelMatrix();

	GLuint MatrixID = glGetUniformLocation(myShaderLibrary->GetProgramID(), "MVP");

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

	glViewport(0, 0, aWidth, aHeight);
	glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(myVertexArrayID);

	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
}
