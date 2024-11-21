#include <GNoise.h>

void GNoise::defines()
{
}

void GNoise::init()
{
    perlin = new PerlinGenerator();
    perlin->generate_perlin(1920, 1080, 5.0f);
}

void GNoise::update()
{
}

void GNoise::clean()
{
}