#pragma once
#include <ECS.h>
#include <GarinIO.h>
#include <GarinNatives.h>
#include <ShaderCompiler.h>
#include <TextureManager.h>
#include <Model.h>
#include <Graphics.h>

class ModelComponent : public Component
{
public:
    string model_path;
    Shader *scene_shader;
    TextureManager *texture_normal;
    TextureManager *texture_sampler;

    Model *model = nullptr;
    Shader *get_mesh_shader();
    void init() override;
    void update() override;
    void draw() override;
    void set_model(string path_load);
    std::string serialize() override;
};
