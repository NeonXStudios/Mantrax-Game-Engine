#pragma once
#include "../EngineUIBehaviour.h"
#include <GarinGraphics.h>
#include <GarinEvents.h>
#include <GarinMaths.h>
#include <EventSystem.h>
#include <RenderPipeline.h>
#include <EngineUI.h>

#include <ImGuizmo.h>

class SceneDataView {
public:
    int window_id;
    Entity* found_object;

    ImVec2 imageSizeSCENE;
    ImVec2 imagePosition;
    glm::vec2 WorldPoint = glm::vec2(0, 0);
    glm::vec2 ScreenPoint = glm::vec2(0, 0);

    Hierarchy* hierarchy = new Hierarchy();

    bool ignoreGui = false;
    bool right_click_held = false;
    bool show_script_popup = false;
    bool is_open;


    Scene* work_scene;
    void close_window();

    SceneDataView();
    void draw();
};

class SceneView : public EngineUIBehaviour
{
public:
    std::vector<SceneDataView*> windows_data = std::vector<SceneDataView*>();
    ImVec2 imageSizeSCENE;
    ImVec2 imagePosition;
    glm::vec2 WorldPoint = glm::vec2(0, 0);
    glm::vec2 ScreenPoint = glm::vec2(0, 0);
    Entity* found_object;

    bool ignoreGui = false;
    bool right_click_held = false;
    bool show_script_popup = false;

    void on_draw() override;
};