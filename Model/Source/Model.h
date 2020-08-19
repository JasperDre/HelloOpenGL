#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <string>

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

	std::vector<float> vbos;

	const std::vector<Mesh>& GetMeshes() const { return myMeshes; }
	const std::vector<std::string>& GetTextureReferences() const { return myTextureReferences; }
	const glm::mat4 GetModelMatrix() const { return myModelMatrix; }
	const glm::vec3 GetPosition() const { return myPosition; }

private:
	void CalculateNormals(float aNormals[3], float aVertex0[3], float aVertex1[3], float aVertex2[3]);

private:
	std::vector<Mesh> myMeshes;
	std::vector<std::string> myTextureReferences;
	glm::mat4 myModelMatrix;
	glm::vec3 myPosition;
};
