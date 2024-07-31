#pragma once
#include <GarinGraphics.h>
#include <GarinIO.h>
#include <Core.h>

class GARINLIBS_API CanvasManager
{
public:
    void init_ui();
    void render_ui();

private:
    unsigned int VAO, VBO, EBO;
    unsigned int textureID;
    Shader *shaderpr;
};