#pragma once
#include "UIComponent.h"

class MenuUI : public UIComponent
{
public:
    bool right_click_held = false;
    bool show_script_popup = false;

    void draw(Scene *scene) override;
};