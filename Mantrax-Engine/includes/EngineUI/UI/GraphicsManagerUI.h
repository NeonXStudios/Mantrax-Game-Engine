#pragma once
#include "../EngineUIBehaviour.h"
#include <AppSettings.h>
#include <Core.h>

class GraphicsManagerUI : public EngineUIBehaviour
{
public:
    bool show_script_popup = false;
    char script_name[128] = "";
    std::string selected_item;

    void on_draw() override;
};