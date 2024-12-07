#pragma once
#include <GarinGraphics.h>
#include <GarinIO.h>
#include <GarinMaths.h>
#include <Core.h>
#include <UIBehaviour.h>
#include <Text.h>

class GARINLIBS_API CanvasManager
{
public:
    float width = 1920.0f;
    float height = 1080.0f;
    float zoom = 0.043f;

    std::vector<UIBehaviour *> ui = std::vector<UIBehaviour *>();

    void init_ui();
    void render_ui();

    // UI COMPONENTS
    UIBehaviour *add_new_ui_component_type_text();
};