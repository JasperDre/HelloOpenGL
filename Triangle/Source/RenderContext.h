#pragma once

class Shader;

class RenderContext
{
public:
	RenderContext();
	~RenderContext();

	void PrintDebugInfo();
	void CompileShaders();
	void GenerateVertexArrayObject();
	void GenerateVertexBufferObject();
	void Render(int aWidth, int aHeight);

private:
	Shader* myShader;
	unsigned int myVertexArrayID;
	unsigned int myVertexBufferID;
};
