#include "Model.h"
#include "GLError.h"
#include "Texture.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <algorithm>

Model::Model(const std::string aPath)
    : myModelMatrix(1.0f)
    , myPosition(0.0f)
    , myVertexBufferObject(0)
{
	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warning;
	std::string error;

    printf("Loading %s\n", aPath.c_str());
    std::size_t found = aPath.find_last_of("/\\");
    std::string baseDirectory = aPath.substr(0, found + 1);
    if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, aPath.c_str(), baseDirectory.c_str(), true))
	{
		printf("Failed to load model: %s\n", aPath.c_str());
		return;
	}

    if (!warning.empty())
    {
        printf("%s\n", warning.c_str());
    }

    if (!error.empty())
    {
        printf("%s\n", error.c_str());
    }

    printf("Number of shapes %i\n", static_cast<int>(shapes.size()));
    printf("Number of materials %i\n", static_cast<int>(materials.size()));

    for (size_t shapeIndex = 0; shapeIndex < shapes.size(); shapeIndex++)
    {
        Mesh mesh;
        size_t indexOffset = 0;

        for (size_t faceIndex = 0; faceIndex < shapes[shapeIndex].mesh.num_face_vertices.size(); faceIndex++)
        {
            int numberOfVertices = shapes[shapeIndex].mesh.num_face_vertices[faceIndex];
            for (size_t vertexIndex = 0; vertexIndex < numberOfVertices; vertexIndex++)
            {
                Vertex vertex;
                glm::vec3 vertexPosition;
                tinyobj::index_t index = shapes[shapeIndex].mesh.indices[indexOffset + vertexIndex];
                vertexPosition.x = attributes.vertices[3 * index.vertex_index + 0];
                vertexPosition.y = attributes.vertices[3 * index.vertex_index + 1];
                vertexPosition.z = attributes.vertices[3 * index.vertex_index + 2];

                glm::vec3 normal(0.0f);
                if (attributes.normals.size() > 0)
                {
                    normal.x = attributes.normals[3 * index.normal_index + 0];
                    normal.y = attributes.normals[3 * index.normal_index + 1];
                    normal.z = attributes.normals[3 * index.normal_index + 2];
                }

                glm::vec2 textureCoordinates(0.0f);
                if (attributes.texcoords.size() > 0)
                {
                    textureCoordinates.x = attributes.texcoords[2 * index.texcoord_index + 0];
                    textureCoordinates.y = attributes.texcoords[2 * index.texcoord_index + 1];
                }

                glm::vec3 color(0.0f);
                if (attributes.colors.size() > 0)
                {
                    color.x = attributes.colors[3 * index.vertex_index + 0];
                    color.y = attributes.colors[3 * index.vertex_index + 1];
                    color.z = attributes.colors[3 * index.vertex_index + 2];
                }

                vertex.myPosition = vertexPosition;
                vertex.myNormal = normal;
                vertex.myColor = color;
                vertex.myTextureCoordinates = textureCoordinates;

                mesh.myVertices.push_back(vertex);
            }

            indexOffset += numberOfVertices;
        }

        mesh.myNumberOfTriangles = (mesh.myVertices.size() * 10) / ((3 + 3 + 2 + 2) * 3);
        myMeshes.push_back(mesh);
    }

    std::vector<std::string> textureReferences;

	for (size_t materialIndex = 0; materialIndex < materials.size(); materialIndex++)
	{
		tinyobj::material_t* material = &materials[materialIndex];
		if (!material->diffuse_texname.empty())
		{
            std::string path = baseDirectory + material->diffuse_texname;
            if (!std::any_of(textureReferences.begin(), textureReferences.end(), [path](std::string aTextureReference) { return aTextureReference == path; }))
            {
                textureReferences.push_back(baseDirectory + material->diffuse_texname);
            }
		}
	}

    for (size_t i = 0; i < textureReferences.size(); i++)
    {
        Texture* texture = new Texture(textureReferences[i]);
        myTextures.push_back(texture);
    }

    printf("Number of meshes %i\n", static_cast<int>(myMeshes.size()));
    printf("Number of textures %i\n", static_cast<int>(myTextures.size()));

    if (myMeshes.size() > 0)
    {
        printf("Number of triangles %i\n", myMeshes[0].myNumberOfTriangles);
        printf("Number of vertices %i\n", static_cast<int>(myMeshes[0].myVertices.size()));
        printf("Loaded %s\n", aPath.c_str());
    }
}

Model::~Model()
{
    glDeleteBuffers((myMeshes[0].myVertices.size() * 10) * sizeof(float), &myVertexBufferObject);
    myMeshes.clear();
    myTextures.clear();
}
