#pragma once
#include <ECS.h>
#include <GarinMaths.h>
#include <GarinGraphics.h>
#include "Core.h"

class GARINLIBS_API GMaterial : public Component
{
public:
    Shader *p_shader;
    Shader *p_gbuffer_shader;
    Shader *p_deferred_shader;

    void defines() override;
    void init() override;
    void update() override;
    void clean() override;
};
