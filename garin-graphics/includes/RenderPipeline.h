#pragma once

#include <iostream>
#include <Core.h>
#include <GarinGraphics.h>
#include "GBuffer.h"
#include "GeometryPass.h"
#include "LightingPass.h"
#include "../includes/CanvasManager.h"

class GARINLIBS_API RenderPipeline
{
public:
    static std::unordered_set<int> layers_to_render;
    static CanvasManager *canvas;

    static Shader *geometryShader;
    static Shader *lightingShader;
    
    static std::vector<ModelComponent *> renderables;

    static void init();
    static void render();
    static void delete_from_render(ModelComponent *renderable);
    static void addLayer(int layer);
    static void removeLayer(int layer);
};