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
    ADD_UI_BEHAVIOUR(MaterialMaker);

    get_component<CompilerView>()->is_open = false;
    get_component<MaterialMaker>()->is_open = false;
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
    std::string id = "Main##1234";
    ImGui::Begin(id.c_str());
    ImGuiID dockspace_id = ImGui::GetID(id.c_str());
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

    for (EngineUIBehaviour *ui_g : ui_s)
    {
        if (ui_g->is_open)
        {
            ui_g->on_draw();
        }
    }
    ImGui::End();
}
