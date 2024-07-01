#pragma once
#include <ECS.h>
#include <GarinMaths.h>
#include <GarinGraphics.h>

class GMaterial : public Component
{
public:
    Shader *p_shader;

    void defines() override;
    void init() override;
    void update() override;
};
