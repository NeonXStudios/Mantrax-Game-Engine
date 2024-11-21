#pragma once
#include "UIComponent.h"
#include <GarinEvents.h>
#include <ImGuizmo.h>
#include <Core.h>
#include "gamescene.h"

class gamescene;
class GARINLIBS_API SceneUI : public UIComponent
{
public:
    gamescene *game;
    ImVec2 imageSizeSCENE;
    ImVec2 imagePosition;
    glm::vec2 WorldPoint = glm::vec2(0, 0);

    bool ignoreGui = false;
    bool right_click_held = false;
    bool show_script_popup = false;

    void draw(Entity *select_obj) override;
};
