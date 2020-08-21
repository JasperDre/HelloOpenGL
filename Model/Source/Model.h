#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <string>

class Texture;

struct Vertex
{
	float myPosition[3];
	float myNormal[3];
};

struct Mesh
{
	std::vector<Vertex> myVertices;
};

class Model
{
public:
	Model(const std::string aPath);
	~Model();

	const std::vector<Mesh>& GetMeshes() const { return myMeshes; }
	const glm::mat4 GetModelMatrix() const { return myModelMatrix; }
	const glm::vec3 GetPosition() const { return myPosition; }

public:
	unsigned int myVertexArrayObject;
	unsigned int myVertexBufferObject;

private:
	std::vector<Mesh> myMeshes;
	glm::mat4 myModelMatrix;
	glm::vec3 myPosition;
};
