#ifndef MODELFBX_H
#define MODELFBX_H
#include <fbxsdk.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <Core.h>
#include <stb_image.h> 

class GARINLIBS_API FBXModelComponent
{
private:
    FbxManager *manager;
    FbxScene *scene;
    std::vector<GLuint> vaoList;
    std::vector<GLuint> vboList;
    std::vector<GLuint> textureList;
    std::vector<int> verticesCountList;

    void InitializeSdk()
    {
        manager = FbxManager::Create();
        if (!manager)
        {
            std::cerr << "Error: No se pudo crear el FbxManager.\n";
            exit(1);
        }
        FbxIOSettings *ios = FbxIOSettings::Create(manager, IOSROOT);
        manager->SetIOSettings(ios);
    }

    bool LoadScene(const char *filename)
    {
        FbxImporter *importer = FbxImporter::Create(manager, "");
        if (!importer->Initialize(filename, -1, manager->GetIOSettings()))
        {
            std::cerr << "Error al inicializar el importador: " << importer->GetStatus().GetErrorString() << "\n";
            return false;
        }
        scene = FbxScene::Create(manager, "myScene");
        if (!importer->Import(scene))
        {
            std::cerr << "Error al importar la escena: " << importer->GetStatus().GetErrorString() << "\n";
            return false;
        }
        importer->Destroy();

        FbxGeometryConverter converter(manager);
        converter.Triangulate(scene, true);

        return true;
    }

    GLuint LoadTexture(const char *filename)
    {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cerr << "Failed to load texture: " << filename << "\n";
        }
        stbi_image_free(data);

        return textureID;
    }

    void ProcessNode(FbxNode *node)
    {
        if (!node)
            return;

        FbxNodeAttribute *attribute = node->GetNodeAttribute();
        if (attribute && attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            ProcessMesh(node->GetMesh());
        }

        for (int i = 0; i < node->GetChildCount(); i++)
        {
            ProcessNode(node->GetChild(i));
        }
    }

    void ProcessMesh(FbxMesh *mesh)
    {
        std::vector<float> vertices;
        std::vector<float> uvs;

        int polygonCount = mesh->GetPolygonCount();
        FbxStringList uvSetNames;
        mesh->GetUVSetNames(uvSetNames);
        const char *uvSetName = uvSetNames.GetCount() > 0 ? uvSetNames.GetStringAt(0) : "";

        FbxGeometryElementUV *uvElement = mesh->GetElementUV(uvSetName);
        if (!uvElement)
        {
            std::cerr << "No UV element found for UV set: " << uvSetName << std::endl;
            return;
        }

        auto mappingMode = uvElement->GetMappingMode();
        auto referenceMode = uvElement->GetReferenceMode();

        for (int i = 0; i < polygonCount; i++)
        {
            int vertexCount = mesh->GetPolygonSize(i);
            for (int j = 0; j < vertexCount; j++)
            {
                int ctrlPointIndex = mesh->GetPolygonVertex(i, j);
                FbxVector4 vertex = mesh->GetControlPointAt(ctrlPointIndex);
                vertices.push_back(static_cast<float>(vertex[0]));
                vertices.push_back(static_cast<float>(vertex[1]));
                vertices.push_back(static_cast<float>(vertex[2]));

                FbxVector2 uv(0.0, 0.0);
                bool unmapped = true;

                if (mappingMode == FbxGeometryElement::eByPolygonVertex)
                {
                    int uvIndex = mesh->GetTextureUVIndex(i, j);
                    if (referenceMode == FbxGeometryElement::eDirect)
                    {
                        uv = uvElement->GetDirectArray().GetAt(uvIndex);
                    }
                    else if (referenceMode == FbxGeometryElement::eIndexToDirect)
                    {
                        int index = uvElement->GetIndexArray().GetAt(uvIndex);
                        uv = uvElement->GetDirectArray().GetAt(index);
                    }
                }
                else if (mappingMode == FbxGeometryElement::eByControlPoint)
                {
                    if (referenceMode == FbxGeometryElement::eDirect)
                    {
                        uv = uvElement->GetDirectArray().GetAt(ctrlPointIndex);
                    }
                    else if (referenceMode == FbxGeometryElement::eIndexToDirect)
                    {
                        int index = uvElement->GetIndexArray().GetAt(ctrlPointIndex);
                        uv = uvElement->GetDirectArray().GetAt(index);
                    }
                    unmapped = false;
                }

                if (!unmapped)
                {
                    float correctedU = static_cast<float>(uv[0]);
                    float correctedV = static_cast<float>(uv[1]);

                    uvs.push_back(correctedU);
                    uvs.push_back(correctedV);

                    std::cout << "UV: (" << correctedU << ", " << correctedV << ")\n";
                }
                else
                {
                    std::cerr << "UV is unmapped at polygon " << i << " vertex " << j << std::endl;
                    uvs.push_back(0.0f);
                    uvs.push_back(0.0f);
                }
            }
        }


        GLuint vao, vboVertices, vboUVs;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vboVertices);
        glGenBuffers(1, &vboUVs);

        vaoList.push_back(vao);
        vboList.push_back(vboVertices);
        vboList.push_back(vboUVs);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, vboUVs);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0); // Corregido el stride
        glEnableVertexAttribArray(1);

        GLuint texture = LoadTexture("assets/Ground054_1K-PNG_Color.png");
        textureList.push_back(texture);

        glBindTexture(GL_TEXTURE_2D, texture);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        verticesCountList.push_back(vertices.size() / 3);
    }

public:
    FBXModelComponent(const char *filename)
    {
        InitializeSdk();
        if (!LoadScene(filename))
        {
            std::cerr << "Error: No se pudo cargar el archivo FBX.\n";
            return;
        }
        ProcessNode(scene->GetRootNode());
    }

    ~FBXModelComponent()
    {
        for (auto vao : vaoList)
        {
            glDeleteVertexArrays(1, &vao);
        }
        for (auto vbo : vboList)
        {
            glDeleteBuffers(1, &vbo);
        }
        for (auto tex : textureList)
        {
            glDeleteTextures(1, &tex);
        }
        if (manager)
        {
            manager->Destroy();
        }
    }

    void Draw()
    {
        for (size_t i = 0; i < vaoList.size(); i++)
        {
            GLuint vao = vaoList[i];
            int verticesCount = verticesCountList[i];
            GLuint texture = textureList[i];

            glBindVertexArray(vao);

            glBindTexture(GL_TEXTURE_2D, texture);

            glDrawArrays(GL_TRIANGLES, 0, verticesCount);
        }

        glBindVertexArray(0);
    }
};
#endif
