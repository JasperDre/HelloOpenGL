#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <string>

class Texture;

struct Vertex
{
	glm::vec3 myPosition;
	glm::vec3 myNormal;
	glm::vec3 myColor;
	glm::vec2 myTextureCoordinates;
};

struct Mesh
{
	std::vector<Vertex> myVertices;
	std::vector<int> myIndices;
	int myNumberOfTriangles;
	int myMaterialID;
};

class Model
{
public:
	Model(const std::string aPath);
	~Model();

	const std::vector<Mesh>& GetMeshes() const { return myMeshes; }
	const std::vector<Texture*>& GetTextures() const { return myTextures; }
	const glm::mat4 GetModelMatrix() const { return myModelMatrix; }
	const glm::vec3 GetPosition() const { return myPosition; }

public:
	unsigned int myVertexArrayObject;
	unsigned int myVertexBufferObject;

private:
	std::vector<Mesh> myMeshes;
	std::vector<Texture*> myTextures;
	glm::mat4 myModelMatrix;
	glm::vec3 myPosition;
};
