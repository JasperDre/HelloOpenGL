#pragma once

#include <glm/mat4x4.hpp>

class ShaderLibrary;
class Camera;

class RenderContext
{
public:
	RenderContext();
	~RenderContext();

	void Render(int aWidth, int aHeight);

private:
	void PrintDebugInfo();
	void CompileShaders();
	void GenerateVertexArrayObject();
	void GenerateVertexBufferObject();
	void BindBuffer();

private:
	ShaderLibrary* myShaderLibrary;
	Camera* myCamera;
	glm::mat4 myModel;
	unsigned int myVertexArrayID;
	unsigned int myVertexBufferID;
};
