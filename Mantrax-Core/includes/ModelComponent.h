#pragma once
#include <ECS.h>
#include <GarinIO.h>
#include <GarinNatives.h>
#include <ShaderCompiler.h>
#include <TextureManager.h>
#include "GMaterial.h"
#include <Model.h>
#include <Graphics.h>
#include "Core.h"
#include <FBXModelComponent.h>

class GARINLIBS_API ModelComponent : public Component
{
public:
    string model_path;

    Model *model = nullptr;
    Shader *get_mesh_shader();
    void set_model(string path_load);

    void defines() override;
    void init() override;
    void update() override;
    void draw() override;
    void clean() override;
    std::string serialize() override;
};
