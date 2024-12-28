#include <PerlinGenerator.h>

#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>

void PerlinGenerator::generate_perlin(int width, int height, float scale)
{
    noiseData.resize(width * height);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float noiseValue = stb_perlin_noise3(x * scale, y * scale, 0.0f, 0, 0, 0);
            noiseData[y * width + x] = noiseValue;
        }
    }

    create_texture(width, height);
}

void PerlinGenerator::render_noise(int width, int height)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void PerlinGenerator::create_texture(int width, int height)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::vector<unsigned char> textureData(width * height * 4);
    for (int i = 0; i < width * height; ++i)
    {
        float noise = noiseData[i];

        float mapped = (noise * 0.5f) + 0.5f;
        if (mapped < 0.0f)
            mapped = 0.0f;
        if (mapped > 1.0f)
            mapped = 1.0f;

        unsigned char value = static_cast<unsigned char>(mapped * 255.0f);

        textureData[i * 4 + 0] = value;
        textureData[i * 4 + 1] = value;
        textureData[i * 4 + 2] = value;
        textureData[i * 4 + 3] = 255;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int PerlinGenerator::get_texture()
{
    return textureID;
}