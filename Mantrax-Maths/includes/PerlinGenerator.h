#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <Core.h>
#include <stb_perlin.h>

class GARINLIBS_API PerlinGenerator
{
public:
    GLuint textureID;
    std::vector<float> noiseData;

    void generate_perlin(int width, int height, float scale);

    void render_noise(int width, int height);

    unsigned int get_texture();

private:
    void create_texture(int width, int height);
};
