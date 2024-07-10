#pragma once
#include "UIComponent.h"
#include <Core.h>

class GARINLIBS_API MenuUI : public UIComponent
{
public:
    bool right_click_held = false;
    bool show_script_popup = false;

    void draw(Scene *scene) override;
};