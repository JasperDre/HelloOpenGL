#include "Model.h"
#include "GLError.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <algorithm>

Model::Model(const std::string aPath)
    : myModelMatrix(1.0f)
    , myPosition(0.0f)
    , myVertexArrayObject(0)
    , myVertexBufferObject(0)
{
    printf("Loading %s\n", aPath.c_str());

    std::size_t lastSlashIndex = aPath.find_last_of("/\\");
    std::size_t lastDotIndex = aPath.find_last_of(".");
    std::string baseDirectory = aPath.substr(0, lastSlashIndex + 1);
    std::string file = aPath.substr(lastSlashIndex + 1, aPath.length() - lastDotIndex);
    std::string extension = aPath.substr(lastDotIndex + 1, aPath.length());

    if (extension == "obj")
    {
        LoadOBJ(aPath, baseDirectory);
    }
    else if (extension == "fbx")
    {
        LoadFBX(aPath);
    }
}

Model::~Model()
{
    myMeshes.clear();
}

void Model::LoadOBJ(const std::string aPath, const std::string aBaseDirectory)
{
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warning;
    std::string error;

    if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, aPath.c_str(), aBaseDirectory.c_str(), true))
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
                tinyobj::index_t index = shapes[shapeIndex].mesh.indices[indexOffset + vertexIndex];
                vertex.myPosition[0] = attributes.vertices[3 * index.vertex_index + 0];
                vertex.myPosition[1] = attributes.vertices[3 * index.vertex_index + 1];
                vertex.myPosition[2] = attributes.vertices[3 * index.vertex_index + 2];

                if (attributes.normals.size() > 0)
                {
                    vertex.myNormal[0] = attributes.normals[3 * index.normal_index + 0];
                    vertex.myNormal[1] = attributes.normals[3 * index.normal_index + 1];
                    vertex.myNormal[2] = attributes.normals[3 * index.normal_index + 2];
                }

                mesh.myVertices.push_back(vertex);
            }

            indexOffset += numberOfVertices;
        }

        myMeshes.push_back(mesh);
    }

    printf("Number of meshes %i\n", static_cast<int>(myMeshes.size()));

    if (myMeshes.size() > 0)
    {
        printf("Number of vertices %i\n", static_cast<int>(myMeshes[0].myVertices.size()));
        printf("Loaded %s\n", aPath.c_str());
    }
}

void Model::LoadFBX(const std::string aPath)
{
}
