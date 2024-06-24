#include "../includes/FilesUI.h"

void FilesUI::draw(Entity *owner, EditorConfigs *p_configs)
{
    EditorConfigs *configs = p_configs;
    ImGui::Begin("Scripts");
    ShowDirectoryTree(FileManager::get_execute_path() + configs->current_proyect + "/clscpp");

    if (ImGui::BeginPopupContextWindow("AssetsPopup", ImGuiMouseButton_Right))
    {
        if (ImGui::MenuItem("Create Class"))
        {
            show_script_popup = true;
        }

        ImGui::EndPopup();
    }

    if (ImGui::Button("Open Class Proyect"))
    {
        std::filesystem::current_path(FileManager::get_execute_path() + configs->current_proyect + "/clscpp");

        int result = system("code .");
    }

    ImGui::End();

    if (show_script_popup)
    {
        ImGui::OpenPopup("Create Script");
    }

    if (ImGui::BeginPopupModal("Create Script", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Class Name:");
        ImGui::InputText("##scriptname", script_name, IM_ARRAYSIZE(script_name));

        if (ImGui::Button("Create"))
        {
            create_script_files(script_name);
            show_script_popup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            show_script_popup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}