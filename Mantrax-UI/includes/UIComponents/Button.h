#pragma once
#include <GarinGraphics.h>
#include <UIBehaviour.h>
#include "../UIBehaviour.h"

class Button : public UIBehaviour
{
public:
    unsigned int texture_id;
    Shader *p_shader;

    void init() override;
    void draw() override;
};