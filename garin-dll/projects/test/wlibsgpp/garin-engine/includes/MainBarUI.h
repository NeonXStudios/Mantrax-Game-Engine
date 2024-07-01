#pragma once
#include "UIComponent.h"

class MainBarUI : public UIComponent
{
public:
    bool show_new_scene_popup = false;
    char new_scene_name[128] = "";

    void draw(Entity *select_obj) override;

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
                Graphics::get_current_scene()->load_scene(new_scene_name);
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
};