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

void AssetsUI::ShowDirectoryTree(const std::filesystem::path &path)
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

                        drawer_files(extension, base_name);
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
            std::string extension = "";
            std::string base_name = "";
            if (entry.path().extension() != ".cpp" && entry.path().extension() != ".h")
            {
                if (ImGui::Selectable(entry.path().filename().string().c_str(), selected_item == entry.path().string()))
                {

                    double current_time = ImGui::GetTime();
                    if (current_time - last_click_time <= double_click_time)
                    {
                        // Doble clic detectado
                        selected_item = entry.path().string();
                        std::filesystem::path selected_path(selected_item);
                        extension = selected_path.extension().string();
                        base_name = selected_path.stem().string();

                        drawer_files(extension, base_name);
                        std::cout << "Archivo doble clic: " << selected_item << std::endl;
                    }
                    else
                    {
                        // Solo primer clic
                        last_click_time = current_time;
                    }
                }
            }

            selected_item = entry.path().string();
            std::filesystem::path selected_path(selected_item);
            extension = selected_path.extension().string();
            base_name = selected_path.stem().string();
            drawer_files_drag(extension, base_name);
        }
    }
}

void AssetsUI::drawer_files(std::string extension, std::string file_name)
{
    if (extension == ".scene")
    {
        Graphics::get_current_scene()->load_scene(file_name);
    }
    else if (extension == ".fbx")
    {
        EditorGUI::Drag("MODELCLASS", file_name);
    }
}

void AssetsUI::drawer_files_drag(std::string extension, std::string file_name)
{
    if (extension == ".fbx" || extension == ".obj")
    {
        EditorGUI::Drag("MODELCLASS", file_name);
    }
    else if (extension == ".mp3" || extension == ".wav")
    {
        EditorGUI::Drag("AUDIOCLASS", file_name);
    }
    else if (extension == ".png" || extension == ".jpg")
    {
        EditorGUI::Drag("IMAGECLASS", file_name);
    }
    else if (extension == ".material" || extension == ".mtl")
    {
        EditorGUI::Drag("MATERIALCLASS", file_name);
    }
    else if (extension == ".material" || extension == ".mtl")
    {
        EditorGUI::Drag("MATERIALCLASS", file_name);
    }
    else if (extension == ".glsl" || extension == ".shader" || extension == ".vs" || extension == ".fs")
    {
        EditorGUI::Drag("SHADERCLASS", file_name);
    }
}