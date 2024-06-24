#include "../includes/AssetsUI.h"

void AssetsUI::draw(EditorConfigs *p_configs)
{
    configs = p_configs;
    ImGui::Begin("Assets");
    ShowDirectoryTree(FileManager::get_execute_path() + configs->current_proyect + "/assets");

    if (ImGui::IsWindowHovered())
    {
        if (ImGui::BeginPopupContextWindow("AssetsPopup", ImGuiMouseButton_Right))
        {
            if (ImGui::MenuItem("Create Script"))
            {
                show_script_popup = true;
            }

            if (ImGui::MenuItem("Create Scene"))
            {
                std::cout << "Crear Escena seleccionado" << std::endl;
            }
            ImGui::EndPopup();
        }
    }

    ImGui::End();
}