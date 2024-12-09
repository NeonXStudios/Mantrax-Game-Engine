#pragma once
#include "UIComponent.h"
#include <GarinEvents.h>
#include <ImGuizmo.h>
#include <Core.h>
#include "EngineUI.h"
#include <UIComponentsBehaviour.h>

class EngineUI;
class GARINLIBS_API SceneUI : public UIComponent
{
public:
    EngineUI *game;
    ImVec2 imageSizeSCENE;
    ImVec2 imagePosition;
    glm::vec2 WorldPoint = glm::vec2(0, 0);
    glm::vec2 ScreenPoint = glm::vec2(0, 0);

    bool ignoreGui = false;
    bool right_click_held = false;
    bool show_script_popup = false;

    void draw(Entity *select_obj) override;
};
