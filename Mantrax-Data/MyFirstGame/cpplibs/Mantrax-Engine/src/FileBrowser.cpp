// FileBrowser.cpp
#include "../includes/FileBrowser.h"
#include <imgui.h>

std::string FileBrowser::currentDirectory = "E:/Github/Garin/bin/projects/test/assets";
std::string FileBrowser::fileExtension = ".fbx";
std::optional<std::string> FileBrowser::selectedFilePath;

std::optional<std::string> FileBrowser::DrawFileBrowser()
{

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImVec2 popupSize = ImVec2(800, 600);
    ImGui::SetNextWindowSize(popupSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("File Browser", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::Button(".."))
        {
            currentDirectory = fs::path(currentDirectory).parent_path().string();
        }

        ImGui::Separator();

        for (const auto &entry : fs::directory_iterator(currentDirectory))
        {
            bool isSelected = selectedFilePath.has_value() && selectedFilePath.value() == entry.path().string();
            if (entry.is_directory())
            {
                if (ImGui::Selectable((entry.path().filename().string() + "/").c_str(), false, ImGuiSelectableFlags_DontClosePopups))
                {
                    currentDirectory = entry.path().string();
                }
            }
            else if (entry.is_regular_file() && entry.path().extension() == fileExtension)
            {
                if (ImGui::Selectable(entry.path().filename().string().c_str(), isSelected, ImGuiSelectableFlags_DontClosePopups))
                {
                    selectedFilePath = entry.path().string();
                }
            }
        }

        if (ImGui::Button("Select"))
        {
            if (selectedFilePath.has_value())
            {
                std::cout << "RUTA SELECCIONADA: " << selectedFilePath.value() << std::endl;
                ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
                return selectedFilePath;
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    return std::nullopt;
}
