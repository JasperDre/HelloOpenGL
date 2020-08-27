#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <assimp/vector2.h>
#include <assimp/vector3.h>

struct Vertex
{
	glm::vec3 myPosition;
	glm::vec3 myNormal;
	glm::vec2 myTextureCoordinates;

	bool operator==(const Vertex& aOther) const
	{
		return myPosition == aOther.myPosition && myNormal == aOther.myNormal && myTextureCoordinates == aOther.myTextureCoordinates;
	}
};

namespace std
{
	template<> struct hash<Vertex>
	{
		size_t operator()(Vertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.myPosition) ^ (hash<glm::vec3>()(vertex.myNormal) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.myTextureCoordinates) << 1);
		}
	};
}

static inline const glm::vec3& CastToVec3(const aiVector3D& aVector3D) { return glm::vec3(aVector3D.x, aVector3D.y, aVector3D.z); }
static inline const glm::vec2& CastToVec2(const aiVector2D& aVector2D) { return glm::vec2(aVector2D.x, aVector2D.y); }
