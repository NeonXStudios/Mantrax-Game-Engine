#include <UIMasterDrawer.h>
#include <imgui.h>
#include <imgui_internal.h>

#include <vector>
#include <string>
#include <optional>

void UIMasterDrawer::register_uis()
{
    ADD_UI_BEHAVIOUR(Hierarchy);
    ADD_UI_BEHAVIOUR(MainBar);
    ADD_UI_BEHAVIOUR(FileBar);
    ADD_UI_BEHAVIOUR(AssetsFiles);
    ADD_UI_BEHAVIOUR(Inspector);
    ADD_UI_BEHAVIOUR(SceneView);
    ADD_UI_BEHAVIOUR(EngineSettings);
    ADD_UI_BEHAVIOUR(AnimatorView);
    ADD_UI_BEHAVIOUR(UIEditor);
    ADD_UI_BEHAVIOUR(CompilerView);

    get_component<CompilerView>()->is_open = false;
    get_component<FileBar>()->is_open = false;
    get_component<Hierarchy>()->is_open = true;
    get_component<AssetsFiles>()->is_open = true;
    get_component<Inspector>()->is_open = true;
    get_component<SceneView>()->is_open = true;
    get_component<EngineSettings>()->is_open = false;
    get_component<AnimatorView>()->is_open = false;
    get_component<UIEditor>()->is_open = false;
}

void UIMasterDrawer::on_draw()
{
    for (EngineUIBehaviour *ui_g : ui_s)
    {
        if (ui_g->is_open)
            ui_g->on_draw();
    }
}
