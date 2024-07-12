#pragma once
#include "UIComponent.h"
#include "../includes/UINotification.h"
#include <GarinNatives.h>
#include <SceneData.h>
#include <Core.h>
#include "gamescene.h"
#include <EditorConfigs.h>

class gamescene;
class GARINLIBS_API MainBarUI : public UIComponent
{
public:
    gamescene *game;
    bool show_new_scene_popup = false;
    char new_scene_name[128] = "";
    EditorConfigs *configs;

    void
    draw(Entity *select_obj) override;

    void ShowNewScenePopup()
    {
        if (show_new_scene_popup)
        {
            ImGui::OpenPopup("New Scene");
        }

        if (ImGui::BeginPopupModal("New Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Enter the name of the new scene:");
            ImGui::InputText("##NewSceneName", new_scene_name, IM_ARRAYSIZE(new_scene_name));

            if (ImGui::Button("OK", ImVec2(120, 0)))
            {
                // Load the new scene
                SceneData::load_scene(new_scene_name);
                show_new_scene_popup = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                show_new_scene_popup = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void recompileAndReloadLibraries();
};