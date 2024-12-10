#pragma once
#include "../EngineUIBehaviour.h"

class SceneView : public EngineUIBehaviour
{
public:
    ImVec2 imageSizeSCENE;
    ImVec2 imagePosition;
    glm::vec2 WorldPoint = glm::vec2(0, 0);
    glm::vec2 ScreenPoint = glm::vec2(0, 0);

    bool ignoreGui = false;
    bool right_click_held = false;
    bool show_script_popup = false;

    void on_draw() override;
};