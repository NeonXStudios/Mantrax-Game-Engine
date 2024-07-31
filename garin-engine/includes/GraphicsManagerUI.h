#pragma once
#include "UIComponent.h"
#include <AppSettings.h>
#include <Core.h>

class GARINLIBS_API GraphicsManagerUI : public UIComponent
{
public:
    EditorConfigs *configs;
    bool show_script_popup = false;
    char script_name[128] = "";
    std::string selected_item;

    void draw(EditorConfigs *p_configs) override;
};