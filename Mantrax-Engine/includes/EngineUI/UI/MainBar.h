#pragma once
#include "../EngineUIBehaviour.h"
#include <UINotification.h>

class MainBar : public EngineUIBehaviour
{
public:
    bool show_new_scene_popup = false;
    char new_scene_name[128] = "";

    void on_draw() override;
    void ShowNewScenePopup();
    void recompileAndReloadLibraries();

    void start_scripts_reload();
};