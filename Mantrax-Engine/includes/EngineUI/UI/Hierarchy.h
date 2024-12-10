#pragma once
#include "../EngineUIBehaviour.h"

class Hierarchy : public EngineUIBehaviour
{
public:
    bool right_click_held = false;
    bool show_script_popup = false;

    void draw_entity_node(Entity *entity);
    void on_draw() override;
};