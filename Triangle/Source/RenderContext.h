#pragma once

class ShaderLibrary;

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
	unsigned int myVertexArrayID;
	unsigned int myVertexBufferID;
};
