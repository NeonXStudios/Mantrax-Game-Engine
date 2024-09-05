#pragma once
#include <GarinGraphics.h>
#include <GarinIO.h>
#include <GarinMaths.h>
#include <Core.h>
#include <UIComponentsBehaviour.h>

class GARINLIBS_API CanvasManager
{
public:
    std::vector<UIComponentsBehaviour *> ui = std::vector<UIComponentsBehaviour *>();

    void init_ui();
    void render_ui();
    UIComponentsBehaviour *add_new_ui_component();
};