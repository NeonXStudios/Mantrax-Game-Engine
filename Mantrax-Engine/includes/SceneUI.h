#pragma once
#include "UIComponent.h"
#include <GarinEvents.h>
#include <ImGuizmo.h>
#include <Core.h>
#include "gamescene.h"
#include <UIComponentsBehaviour.h>

class gamescene;
class GARINLIBS_API SceneUI : public UIComponent
{
public:
    gamescene *game;
    ImVec2 imageSizeSCENE;
    ImVec2 imagePosition;
    glm::vec2 WorldPoint = glm::vec2(0, 0);
    glm::vec2 ScreenPoint = glm::vec2(0, 0);

    bool ignoreGui = false;
    bool right_click_held = false;
    bool show_script_popup = false;

    bool IsMouseOverUIElement(const UIComponentsBehaviour &element, const ImVec2 &mousePos)
    {
        float xMin = element.transform->Position.x;
        float xMax = element.transform->Position.x + element.transform->Scale.x;
        float yMin = element.transform->Position.y;
        float yMax = element.transform->Position.y + element.transform->Scale.y;

        return (mousePos.x >= xMin && mousePos.x <= xMax &&
                mousePos.y >= yMin && mousePos.y <= yMax);
    }

    void draw(Entity *select_obj) override;
};
