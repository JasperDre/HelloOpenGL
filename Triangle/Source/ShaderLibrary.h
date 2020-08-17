#pragma once

class Shader;

class ShaderLibrary
{
public:
	ShaderLibrary();
	~ShaderLibrary();

	void BindShaders();

private:
	void AttachShaders(const Shader& aVertexShader, const Shader& aFragmentShader);

private:
	Shader* myVertexShader;
	Shader* myFragmentShader;
	unsigned int myProgramID;
};
