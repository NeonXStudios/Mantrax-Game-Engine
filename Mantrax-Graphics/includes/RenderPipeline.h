#pragma once

#include <iostream>
#include <Core.h>
#include <GarinGraphics.h>
#include "GBuffer.h"
#include "GeometryPass.h"
#include "LightingPass.h"
#include "../includes/CanvasManager.h"
#include <TextureTarget.h>
#include <camera.h>

class GARINLIBS_API RenderPipeline
{
public:
    static std::unordered_set<int> layers_to_render;
    static CanvasManager *canvas;

    static std::vector<ModelComponent *> renderables;
    static std::vector<TextureTarget *> render_targets;
    static std::vector<Camera *> camera_targets;

    static void init();
    static void render();
    static void render_all_data(glm::mat4 camera_matrix);
    static void delete_from_render(ModelComponent *renderable);
    static void addLayer(int layer);
    static void removeLayer(int layer);
    static Camera *add_camera();
    static TextureTarget *add_render_texture();
};