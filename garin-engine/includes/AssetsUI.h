#pragma once
#include "UIComponent.h"

class AssetsUI : public UIComponent
{
public:
    EditorConfigs *configs;
    bool show_script_popup = false;
    char script_name[128] = "";
    std::string selected_item;

    void draw(EditorConfigs *p_configs) override;
    void ShowDirectoryTree(const std::filesystem::path &path)
    {
        for (const auto &entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                if (ImGui::TreeNode(entry.path().filename().string().c_str()))
                {
                    if (ImGui::Selectable(entry.path().filename().string().c_str(), selected_item == entry.path().string()))
                    {
                        selected_item = entry.path().string();
                        std::cout << "Directorio seleccionado: " << selected_item << std::endl;
                    }

                    ShowDirectoryTree(entry.path());
                    ImGui::TreePop();
                }
            }
            else
            {
                if (ImGui::Selectable(entry.path().filename().string().c_str(), selected_item == entry.path().string()))
                {
                    selected_item = entry.path().string();
                    std::cout << "Archivo seleccionado: " << selected_item << std::endl;
                }
            }
        }
    }
};