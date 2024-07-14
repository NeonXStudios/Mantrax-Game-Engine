#pragma once

#include <iostream>
#include <Core.h>
#include <GarinGraphics.h>
#include "GBuffer.h"
#include "GeometryPass.h"
#include "LightingPass.h"

class GARINLIBS_API RenderPipeline
{
public:
    static Shader *geometryShader;
    static Shader *lightingShader;

    static std::vector<ModelComponent *> renderables;

    static void init();
    static void render();
    static void delete_from_render(ModelComponent *renderable);
};