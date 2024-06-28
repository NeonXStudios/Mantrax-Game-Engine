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
        static double last_click_time = -1.0;
        constexpr double double_click_time = 0.2;

        for (const auto &entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                if (ImGui::TreeNode(entry.path().filename().string().c_str()))
                {
                    if (ImGui::IsItemClicked(0))
                    {
                        double current_time = ImGui::GetTime();
                        if (current_time - last_click_time <= double_click_time)
                        {
                            // Doble clic detectado
                            selected_item = entry.path().string();
                            std::filesystem::path selected_path(selected_item);
                            std::string extension = selected_path.extension().string();
                            std::string base_name = selected_path.stem().string();

                            if (extension == ".scene")
                            {
                                Graphics::get_current_scene()->load_scene(base_name);
                            }
                            std::cout << "Directorio doble clic: " << selected_item << std::endl;
                        }
                        else
                        {
                            // Solo primer clic
                            last_click_time = current_time;
                        }
                    }

                    ShowDirectoryTree(entry.path());
                    ImGui::TreePop();
                }
            }
            else
            {
                if (ImGui::Selectable(entry.path().filename().string().c_str(), selected_item == entry.path().string()))
                {
                    double current_time = ImGui::GetTime();
                    if (current_time - last_click_time <= double_click_time)
                    {
                        // Doble clic detectado
                        selected_item = entry.path().string();
                        std::filesystem::path selected_path(selected_item);
                        std::string extension = selected_path.extension().string();
                        std::string base_name = selected_path.stem().string();

                        if (extension == ".scene")
                        {
                            Graphics::get_current_scene()->load_scene(base_name);
                        }
                        std::cout << "Archivo doble clic: " << selected_item << std::endl;
                    }
                    else
                    {
                        // Solo primer clic
                        last_click_time = current_time;
                    }
                }
            }
        }
    }
};