#pragma once
#include "UIComponent.h"
#include "EngineUI.h"
#include <Core.h>

class EngineUI;

class GARINLIBS_API HierarchyUI : public UIComponent
{
public:
    EngineUI *game;
    bool right_click_held = false;
    bool show_script_popup = false;

    void draw(Entity *select_obj) override;
    void drawEntityNode(Entity *entity);
};
