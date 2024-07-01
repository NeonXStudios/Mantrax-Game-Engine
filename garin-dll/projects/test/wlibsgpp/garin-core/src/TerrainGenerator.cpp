#include "../includes/TerrainGenerator.h"

void TerrainGenerator::init()
{
    setupMesh();

    for (int z = 0; z < terrainDepth; ++z)
    {
        for (int x = 0; x < terrainWidth; ++x)
        {
            float y = generateHeight(x, z);
            vertices.push_back(glm::vec3(x, y, z));
        }
    }

    for (int z = 0; z < terrainDepth - 1; ++z)
    {
        for (int x = 0; x < terrainWidth - 1; ++x)
        {
            int topLeft = (z * terrainWidth) + x;
            int topRight = topLeft + 1;
            int bottomLeft = ((z + 1) * terrainWidth) + x;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    std::cout << "GENERATING TERRAIN" << std::endl;
}

void TerrainGenerator::draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}