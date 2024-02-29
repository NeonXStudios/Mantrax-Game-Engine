#pragma once
#include <iostream>
#include <ECS.h>
#include <GarinCore.h>

class ModelComponent : public Component
{
private:
    string model_path;
    Shader *scene_shader;
    Model *model = nullptr;
    TextureManager *texture_sampler;
    TextureManager *texture_normal;

public:
    Shader *get_mesh_shader();
    void init() override;
    void update() override;
    void set_model(string path_load);
};
