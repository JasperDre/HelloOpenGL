#include "Model.h"
#include "GLError.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Model::Model(const std::string aPath)
    : myModelMatrix(1.0f)
    , myPosition(0.0f)
{
	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warning;
	std::string error;

    printf("Loading %s\n", aPath.c_str());

    std::string baseDirectory = aPath.substr(0, aPath.find_last_of("\\/"));
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

    for (size_t shapeIndex = 0; shapeIndex < shapes.size(); shapeIndex++)
    {
        Mesh mesh;
        size_t indexOffset = 0;
        for (size_t faceIndex = 0; faceIndex < shapes[shapeIndex].mesh.num_face_vertices.size(); faceIndex++)
        {
            int numberOfVerticesInFace = shapes[shapeIndex].mesh.num_face_vertices[faceIndex];
            for (size_t vertexIndex = 0; vertexIndex < numberOfVerticesInFace; vertexIndex++)
            {
                Vertex vertex;
                tinyobj::index_t index = shapes[shapeIndex].mesh.indices[indexOffset + vertexIndex];
                tinyobj::real_t vertexX = attributes.vertices[3 * index.vertex_index + 0];
                tinyobj::real_t vertexY = attributes.vertices[3 * index.vertex_index + 1];
                tinyobj::real_t vertexZ = attributes.vertices[3 * index.vertex_index + 2];
                tinyobj::real_t normalX = attributes.normals[3 * index.normal_index + 0];
                tinyobj::real_t normalY = attributes.normals[3 * index.normal_index + 1];
                tinyobj::real_t normalZ = attributes.normals[3 * index.normal_index + 2];

                vbos.push_back(vertexX);
                vbos.push_back(vertexY);
                vbos.push_back(vertexZ);
                vbos.push_back(normalX);
                vbos.push_back(normalY);
                vbos.push_back(normalZ);

                tinyobj::real_t textureX = 0;
                tinyobj::real_t textureY = 0;

                if (attributes.texcoords.size() > 0)
                {
                    textureX = attributes.texcoords[2 * index.texcoord_index + 0];
                    textureY = attributes.texcoords[2 * index.texcoord_index + 1];
                }
                
                vertex.myTextureCoordinates = glm::vec2(textureX, textureY);
                vbos.push_back(textureX);
                vbos.push_back(textureY);

                tinyobj::real_t red = attributes.colors[3 * index.vertex_index + 0];
                tinyobj::real_t green = attributes.colors[3 * index.vertex_index + 1];
                tinyobj::real_t blue = attributes.colors[3 * index.vertex_index + 2];

                vertex.myPosition = glm::vec3(vertexX, vertexY, vertexZ);
                vertex.myNormal = glm::vec3(normalX, normalY, normalZ);
                vertex.myColor = glm::vec3(red, green, blue);

                mesh.myVertices.push_back(vertex);
            }

            indexOffset += numberOfVerticesInFace;
        }

        mesh.myNumberOfTriangles = vbos.size() / ((3 + 3 + 2) * 3);
        myMeshes.push_back(mesh);
    }
            //tinyobj::index_t idx0 = shape.mesh.indices[3 * faceIndex + 0];
            //tinyobj::index_t idx1 = shape.mesh.indices[3 * faceIndex + 1];
            //tinyobj::index_t idx2 = shape.mesh.indices[3 * faceIndex + 2];

            //int materialID = shape.mesh.material_ids[faceIndex];

            //if ((materialID < 0) || (materialID >= static_cast<int>(materials.size())))
            //{
            //    materialID = materials.size() - 1;
            //}

            //float diffuse[3];
            //for (size_t i = 0; i < 3; i++)
            //{
            //    diffuse[i] = materials[materialID].diffuse[i];
            //}

            //float tc[3][2];
            //if (attributes.texcoords.size() > 0)
            //{
            //    ASSERT(attributes.texcoords.size() > 2 * idx0.texcoord_index + 1);
            //    ASSERT(attributes.texcoords.size() > 2 * idx1.texcoord_index + 1);
            //    ASSERT(attributes.texcoords.size() > 2 * idx2.texcoord_index + 1);

            //    // The division by materials[current_material_id].diffuse_texopt.scale[] solves the problem of textures
            //    // Being stretched on the whole surface. If we want to repeat a texture we need to give the uv coordinates
            //    // a value > 1. In example a value of 2 means the texture is going to be repeated twice
            //    // The scale value ranges from 0.0 to 1.0 so dividing by a number < 0 we're actually increasing the uv values
            //    // and our textures will be repeated accordingly

            //    tc[0][0] = attributes.texcoords[2 * idx0.texcoord_index] / materials[materialID].diffuse_texopt.scale[0];
            //    tc[0][1] = 1.0f - attributes.texcoords[2 * idx0.texcoord_index + 1] / materials[materialID].diffuse_texopt.scale[1];
            //    tc[1][0] = attributes.texcoords[2 * idx1.texcoord_index] / materials[materialID].diffuse_texopt.scale[0];
            //    tc[1][1] = 1.0f - attributes.texcoords[2 * idx1.texcoord_index + 1] / materials[materialID].diffuse_texopt.scale[1];
            //    tc[2][0] = attributes.texcoords[2 * idx2.texcoord_index] / materials[materialID].diffuse_texopt.scale[0];
            //    tc[2][1] = 1.0f - attributes.texcoords[2 * idx2.texcoord_index + 1] / materials[materialID].diffuse_texopt.scale[1];

            //}
            //else
            //{
            //    tc[0][0] = 0.0f;
            //    tc[0][1] = 0.0f;
            //    tc[1][0] = 0.0f;
            //    tc[1][1] = 0.0f;
            //    tc[2][0] = 0.0f;
            //    tc[2][1] = 0.0f;
            //}

            //float v[3][3];
            //for (int k = 0; k < 3; k++)
            //{
            //    int f0 = idx0.vertex_index;
            //    int f1 = idx1.vertex_index;
            //    int f2 = idx2.vertex_index;
            //    assert(f0 >= 0);
            //    assert(f1 >= 0);
            //    assert(f2 >= 0);

            //    v[0][k] = attributes.vertices[3 * f0 + k];
            //    v[1][k] = attributes.vertices[3 * f1 + k];
            //    v[2][k] = attributes.vertices[3 * f2 + k];
            //    bmin[k] = std::min(v[0][k], bmin[k]);
            //    bmin[k] = std::min(v[1][k], bmin[k]);
            //    bmin[k] = std::min(v[2][k], bmin[k]);
            //    bmax[k] = std::max(v[0][k], bmax[k]);
            //    bmax[k] = std::max(v[1][k], bmax[k]);
            //    bmax[k] = std::max(v[2][k], bmax[k]);
            //}

            //float n[3][3];

            ////If the normals are stored in the mesh, load them in
            //if (attributes.normals.size() > 0)
            //{
            //    int f0 = idx0.normal_index;
            //    int f1 = idx1.normal_index;
            //    int f2 = idx2.normal_index;
            //    assert(f0 >= 0);
            //    assert(f1 >= 0);
            //    assert(f2 >= 0);
            //    for (int k = 0; k < 3; k++)
            //    {
            //        n[0][k] = attributes.normals[3 * f0 + k];
            //        n[1][k] = attributes.normals[3 * f1 + k];
            //        n[2][k] = attributes.normals[3 * f2 + k];
            //    }
            //}
            //else
            //{
            //    CalculateNormals(n[0], v[0], v[1], v[2]);
            //    n[1][0] = n[0][0];
            //    n[1][1] = n[0][1];
            //    n[1][2] = n[0][2];
            //    n[2][0] = n[0][0];
            //    n[2][1] = n[0][1];
            //    n[2][2] = n[0][2];
            //}

            //for (int k = 0; k < 3; k++)
            //{
            //    //Start pushing back the vertices and the normals
            //    vbos.push_back(v[k][0]);
            //    vbos.push_back(v[k][1]);
            //    vbos.push_back(v[k][2]);
            //    vbos.push_back(n[k][0]);
            //    vbos.push_back(n[k][1]);
            //    vbos.push_back(n[k][2]);

            //    //Push back the uv coordinates
            //    vbos.push_back(tc[k][0]);
            //    vbos.push_back(tc[k][1]);

            //    vertex.myPosition.x = v[k][0];
            //    vertex.myPosition.y = v[k][1];
            //    vertex.myPosition.z = v[k][2];

            //    vertex.myNormal.x = n[k][0];
            //    vertex.myNormal.y = n[k][1];
            //    vertex.myNormal.z = n[k][2];

            //    mesh.myVertices.push_back(vertex);
        //    }
        //}

        //// Does not support texturing with per-face material
        //if (shape.mesh.material_ids.size() > 0 && shape.mesh.material_ids.size() > shapeIndex)
        //{
        //    mesh.myMaterialID = shape.mesh.material_ids[shapeIndex];
        //}
        //else
        //{
        //    mesh.myMaterialID = materials.size() - 1;
        //}

        //// Here we should have a full vector of data so we can create the VBO for this shape
        //if (vbos.size() > 0)
        //{
        //    mesh.myNumberOfTriangles = vbos.size() / ((3 + 3 + 2) * 3);
        //}

        //myMeshes.push_back(mesh);

	for (size_t materialIndex = 0; materialIndex < materials.size(); materialIndex++)
	{
		tinyobj::material_t* material = &materials[materialIndex];
		if (!material->diffuse_texname.empty())
		{
			if (std::find(myTextureReferences.begin(), myTextureReferences.end(), material->diffuse_texname) != myTextureReferences.end())
			{
				myTextureReferences.push_back(baseDirectory + material->diffuse_texname);
			}
		}
	}

    printf("Number of meshes %i\n", static_cast<int>(myMeshes.size()));
    printf("Number of materials %i\n", static_cast<int>(materials.size()));
    printf("Number of textures %i\n", static_cast<int>(myTextureReferences.size()));

    if (myMeshes.size() > 0)
    {
        printf("Number of triangles %i\n", myMeshes[0].myNumberOfTriangles);
        printf("Number of vertices %i\n", static_cast<int>(myMeshes[0].myVertices.size()));
        printf("Loaded %s\n", aPath.c_str());
    }
}

Model::~Model()
{
}

void Model::CalculateNormals(float aNormals[3], float aVertex0[3], float aVertex1[3], float aVertex2[3])
{
    float v10[3];
    v10[0] = aVertex1[0] - aVertex0[0];
    v10[1] = aVertex1[1] - aVertex0[1];
    v10[2] = aVertex1[2] - aVertex0[2];

    float v20[3];
    v20[0] = aVertex2[0] - aVertex0[0];
    v20[1] = aVertex2[1] - aVertex0[1];
    v20[2] = aVertex2[2] - aVertex0[2];

    aNormals[0] = v20[1] * v10[2] - v20[2] * v10[1];
    aNormals[1] = v20[2] * v10[0] - v20[0] * v10[2];
    aNormals[2] = v20[0] * v10[1] - v20[1] * v10[0];

    float lengthSquared = aNormals[0] * aNormals[0] + aNormals[1] * aNormals[1] + aNormals[2] * aNormals[2];
    if (lengthSquared > 0.0f)
    {
        float length = sqrtf(lengthSquared);

        aNormals[0] /= length;
        aNormals[1] /= length;
    }
}
