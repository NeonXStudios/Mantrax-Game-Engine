#pragma once
#include <ECS.h>
#include <GarinMaths.h>
#include <GarinGraphics.h>
#include "Core.h"

#include <unordered_map>
#include <iostream>
#include <string>
#include <TextureManager.h>

class GARINLIBS_API GMaterial : public Component
{
public:
    Shader *p_shader;
    Shader *p_gbuffer_shader;
    Shader *p_deferred_shader;

    std::unordered_map<std::string, TextureManager *> textures;

    void defines() override;
    void init() override;
    void update() override;
    void clean() override;

    void create_texture(const std::string& name, std::string texture_path);
    TextureManager* get_texture(const std::string& name);
};
