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

struct GARINLIBS_API MaterialSetter
{
    int material_id;
    int mesh_index;
};

class GARINLIBS_API ModelComponent : public Component
{
public:
    string model_path;

    Model *model = nullptr;
    Shader *get_mesh_shader();
    void set_model(string path_load);
    void apply_new_material_to_sub_mesh(MaterialSetter *new_mat);

    void defines() override;
    void init() override;
    void update() override;
    void draw() override;
    void clean() override;
    std::string serialize() override;
};
