#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "ECS.h"
#include "PerlinNoise.hpp"

class TerrainGenerator : public Component
{
public:
    // TerrainGenerator(int width, int depth, unsigned int seed = 1234, float scale = 0.1f, float heightMultiplier = 10.0f)
    //     : terrainWidth(width), terrainDepth(depth), perlin(seed), scale(scale), heightMultiplier(heightMultiplier)
    // {
    //     // generateTerrain();
    //     setupMesh();
    // }

    ~TerrainGenerator()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

public:
    int terrainWidth = 100;
    int terrainDepth = 100;
    float scale = 1.0f;
    float heightMultiplier = 10.0f;
    siv::PerlinNoise perlin = siv::PerlinNoise(1234);

    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    unsigned int VAO, VBO, EBO;

    void init() override;
    void draw() override;

    float generateHeight(int x, int z)
    {
        return perlin.noise2D(x * scale, z * scale) * heightMultiplier;
    }

    void setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};

#endif // TERRAIN_GENERATOR_H
