#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

class Texture;

struct Vertex
{
	glm::vec3 myPosition;
	glm::vec3 myNormal;
	glm::vec3 myColor;
	glm::vec2 myTextureCoordinates;

	bool operator==(const Vertex& aOther) const
	{
		return myPosition == aOther.myPosition && myNormal == aOther.myNormal && myColor == aOther.myColor && myTextureCoordinates == aOther.myTextureCoordinates;
	}
};

namespace std
{
	template<> struct hash<Vertex>
	{
		size_t operator()(Vertex const& aVertex) const
		{
			return ((hash<glm::vec3>()(aVertex.myPosition) ^
					 (hash<glm::vec3>()(aVertex.myColor) << 1)) >> 1) ^
				(hash<glm::vec2>()(aVertex.myTextureCoordinates) << 1);
		}
	};
}

struct Mesh
{
	std::vector<Vertex> myVertices;
	std::vector<unsigned int> myIndices;
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

private:
	bool DoesFileExist(const std::string& aPath);
	void LoadOBJ(const std::string aPath, const std::string aBaseDirectory);
	void LoadFBX(const std::string aPath);

public:
	unsigned int myVertexArrayObject;
	unsigned int myVertexBufferObject;
	unsigned int myElementBufferObject;
	unsigned int myColorBufferObject;
	unsigned int myTextureUnit;

private:
	std::vector<Mesh> myMeshes;
	std::vector<Texture*> myTextures;
	glm::mat4 myModelMatrix;
	glm::vec3 myPosition;
};
