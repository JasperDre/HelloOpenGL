#include "Model.h"
#include "GLError.h"
#include "Texture.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <algorithm>

Model::Model(const std::string aPath)
    : myModelMatrix(1.0f)
    , myPosition(0.0f)
    , myVertexArrayObject(0)
    , myVertexBufferObject(0)
{
    if (!DoesFileExist(aPath))
    {
        printf("%s does not exist\n", aPath.c_str());
        return;
    }

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

bool Model::DoesFileExist(const std::string& aPath)
{
    FILE* file;
    errno_t error = fopen_s(&file, aPath.c_str(), "rb");
    if (error != 0)
    {
        return false;
    }
    
    return true;
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
        printf("Failed to parse %s\n", aPath.c_str());
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

    for (int shapeIndex = 0; shapeIndex < shapes.size(); ++shapeIndex)
    {
        Mesh mesh;
        int indexOffset = 0;

        for (int faceIndex = 0; faceIndex < shapes[shapeIndex].mesh.num_face_vertices.size(); ++faceIndex)
        {
            int numberOfVertices = shapes[shapeIndex].mesh.num_face_vertices[faceIndex];

            for (int vertexIndex = 0; vertexIndex < numberOfVertices; ++vertexIndex)
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

                if (attributes.texcoords.size() > 0)
                {
                    vertex.myTextureCoordinates[0] = attributes.texcoords[2 * index.texcoord_index + 0];
                    vertex.myTextureCoordinates[1] = attributes.texcoords[2 * index.texcoord_index + 1];
                }

                mesh.myVertices.push_back(vertex);
            }

            indexOffset += numberOfVertices;
        }

        myMeshes.push_back(mesh);
    }

    printf("Number of meshes %i\n", static_cast<int>(myMeshes.size()));

    for (int materialIndex = 0; materialIndex < materials.size(); ++materialIndex)
    {
        tinyobj::material_t* material = &materials[materialIndex];

        if (material->diffuse_texname.length() > 0)
        {
            std::string texturePath = aBaseDirectory + material->diffuse_texname;
            if (!DoesFileExist(texturePath))
            {
                break;
            }

            Texture* texture = new Texture(texturePath);
            myTextures.push_back(texture);
        }
    }

    if (myMeshes.size() > 0)
    {
        printf("Number of vertices %i\n", static_cast<int>(myMeshes[0].myVertices.size()));
        printf("Loaded %s\n", aPath.c_str());
    }
}

void Model::LoadFBX(const std::string aPath)
{
    Assimp::Importer importer;

    unsigned int flags = aiProcess_Triangulate;
    if (!importer.ValidateFlags(flags))
    {
        printf("Flags are incompatible\n");
    }

    const aiScene* scene = importer.ReadFile(aPath.c_str(), flags);
    if (!scene)
    {
        printf("Failed to parse %s %s\n", aPath.c_str(), importer.GetErrorString());
        return;
    }

    for (int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
    {
        const aiMesh* aiMesh = scene->mMeshes[meshIndex];
        Mesh mesh;

        for (int faceIndex = 0; faceIndex < aiMesh->mNumFaces; ++faceIndex)
        {
            const aiFace& face = aiMesh->mFaces[faceIndex];

            for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
            {
                const aiVector3D zero3D(0.0f, 0.0f, 0.0f);
                const aiVector3D* position = &(aiMesh->mVertices[face.mIndices[vertexIndex]]);
                const aiVector3D* normal = aiMesh->mNormals ? &(aiMesh->mNormals[face.mIndices[vertexIndex]]) : &zero3D;

                Vertex vertex;
                vertex.myPosition[0] = static_cast<float>(position->x);
                vertex.myPosition[1] = static_cast<float>(position->y);
                vertex.myPosition[2] = static_cast<float>(position->z);

                vertex.myNormal[0] = static_cast<float>(normal->x);
                vertex.myNormal[1] = static_cast<float>(normal->y);
                vertex.myNormal[2] = static_cast<float>(normal->z);

                if (aiMesh->HasTextureCoords(0))
                {
                    vertex.myTextureCoordinates[0] = aiMesh->mTextureCoords[0][face.mIndices[vertexIndex]].x;
                    vertex.myTextureCoordinates[1] = aiMesh->mTextureCoords[0][face.mIndices[vertexIndex]].y;
                }

                mesh.myVertices.push_back(vertex);
            }
        }

        myMeshes.push_back(mesh);
    }
    
    printf("Number of meshes %i\n", static_cast<int>(myMeshes.size()));

    if (scene->HasMaterials())
    {
        for (int materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex)
        {
            const aiMaterial* material = scene->mMaterials[materialIndex];
            aiString texturePath;

            unsigned int numberOfTextures = material->GetTextureCount(aiTextureType_DIFFUSE);

            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0 && material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
            {
                if (!DoesFileExist(texturePath.C_Str()))
                {
                    break;
                }

                Texture* texture = new Texture(texturePath.C_Str());
                myTextures.push_back(texture);
            }
        }
    }

    if (myMeshes.size() > 0)
    {
        printf("Number of vertices %i\n", static_cast<int>(myMeshes[0].myVertices.size()));
        printf("Loaded %s\n", aPath.c_str());
    }
}
