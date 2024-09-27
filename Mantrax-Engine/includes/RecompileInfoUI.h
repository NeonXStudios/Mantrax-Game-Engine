#pragma once
#include <imgui.h>
#include <Core.h>

class GARINLIBS_API RecompileInfoUI
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
