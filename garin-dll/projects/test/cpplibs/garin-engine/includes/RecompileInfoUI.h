#pragma once
#include <imgui.h>

class RecompileInfoUI
{
public:
    static void view_ui_recompile()
    {
        if (ImGui::BeginPopup("Info..."))
        {
            ImGui::Text("Recompiling libraries...");
            if (ImGui::Button("Cerrar"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
};
