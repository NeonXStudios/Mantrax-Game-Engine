#pragma once
#include "UIComponent.h"
#include <ImGuizmo.h>

class SceneUI : public UIComponent
{
public:
    bool ignoreGui = false;
    bool right_click_held = false;
    bool show_script_popup = false;

    void draw(Entity *select_obj) override;
};
