#pragma once
#include "UIComponent.h"
#include <Core.h>
#include <GarinGraphics.h>

class GARINLIBS_API MakerUI : public UIComponent
{
public:
    ImVec2 imageSizeSCENE;
    ImVec2 imagePosition;

    void draw(Scene *scene) override;
};