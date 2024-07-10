#pragma once
#include "UIComponent.h"
#include <GarinEvents.h>
#include <ImGuizmo.h>
#include <Core.h>

class GARINLIBS_API SceneUI : public UIComponent
{
public:
    bool ignoreGui = false;
    bool right_click_held = false;
    bool show_script_popup = false;

    void draw(Entity *select_obj) override;
};
