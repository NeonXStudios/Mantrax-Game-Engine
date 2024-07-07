#pragma once
#include "UIComponent.h"
#include "gamescene.h"

class gamescene;

class HierarchyUI : public UIComponent
{
public:
    gamescene *game;
    bool right_click_held = false;
    bool show_script_popup = false;

    void draw(Entity *select_obj) override;
    void drawEntityNode(Entity *entity);
};
