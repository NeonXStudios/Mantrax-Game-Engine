#include "../includes/AssetsUI.h"
#include <SceneData.h>
#include <CodeEditor.h>

void AssetsUI::draw(EditorConfigs *p_configs)
{
    configs = p_configs;
    ImGui::Begin("Assets", &is_open);
    ShowDirectoryTree(FileManager::get_project_path() + "/assets");

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
            EditorGUI::DrawIcon(IconsManager::FOLDER());

            if (ImGui::TreeNode(entry.path().filename().string().c_str()))
            {
                if (ImGui::IsItemClicked(0))
                {
                    double current_time = ImGui::GetTime();
                    if (current_time - last_click_time <= double_click_time)
                    {
                        selected_item = entry.path().string();
                        std::filesystem::path selected_path(selected_item);
                        std::string extension = selected_path.extension().string();
                        std::string base_name = selected_path.stem().string();

                        drawer_files(extension, base_name, entry.path().string());
                        std::cout << "Directorio doble clic: " << selected_item << std::endl;
                    }
                    else
                    {
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
                if (entry.path().extension() == ".lua")
                {
                    EditorGUI::DrawIcon(IconsManager::LUA());
                }
                else if (entry.path().extension() == ".scene")
                {
                    EditorGUI::DrawIcon(IconsManager::SCENE());
                }
                else if (entry.path().extension() == ".fbx")
                {
                    EditorGUI::DrawIcon(IconsManager::MODEL());
                }
                else if (entry.path().extension() == ".wav" || entry.path().extension() == ".mp3")
                {
                    EditorGUI::DrawIcon(IconsManager::SOUND());
                }
                else
                {
                    if (entry.path().extension() != ".garin")
                        EditorGUI::DrawIcon(IconsManager::UNKNOWN());
                }

                if (entry.path().extension() != ".garin")
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

                            drawer_files(extension, base_name, entry.path().string());
                            std::cout << "Opening File: " << base_name << std::endl;
                        }
                        else
                        {
                            // Solo primer clic
                            last_click_time = current_time;
                        }
                    }
                }
            }

            selected_item = entry.path().string();
            std::filesystem::path selected_path(selected_item);
            extension = selected_path.extension().string();
            base_name = selected_path.stem().string();
            drawer_files_drag(extension, base_name, entry.path().string());
        }
    }
}

void AssetsUI::drawer_files(std::string extension, std::string file_name, std::string file_path_complete)
{
    if (extension == ".scene")
    {
        if (!AppSettings::is_playing)
        {
            SceneData::load_scene(file_name);
        }
    }
    else if (extension == ".slab" || extension == ".lua")
    {
        std::filesystem::current_path(FileManager::get_game_path() + "/assets/");

        std::string open_command = "code . " + file_path_complete;

        int result = system(open_command.c_str());
        // CodeEditor::get_instance().setup_new_editor(file_path_complete);
    }
}

void AssetsUI::drawer_files_drag(std::string extension, std::string file_name, std::string complete_path)
{
    if (extension == ".fbx" || extension == ".obj" || extension == ".dae")
    {
        EditorGUI::Drag("MODELCLASS", GarinIO::GetWithAfterAssetDir(complete_path));
    }
    else if (extension == ".mp3" || extension == ".wav")
    {
        EditorGUI::Drag("AUDIOCLASS", GarinIO::GetWithAfterAssetDir(complete_path));
    }
    else if (extension == ".png" || extension == ".jpg")
    {
        EditorGUI::Drag("IMAGECLASS", GarinIO::GetWithAfterAssetDir(complete_path));
    }
    else if (extension == ".material" || extension == ".mtl")
    {
        EditorGUI::Drag("MATERIALCLASS", GarinIO::GetWithAfterAssetDir(complete_path));
    }
    else if (extension == ".material" || extension == ".mtl")
    {
        EditorGUI::Drag("MATERIALCLASS", GarinIO::GetWithAfterAssetDir(complete_path));
    }
    else if (extension == ".glsl" || extension == ".shader" || extension == ".vs" || extension == ".fs" || extension == ".slab")
    {
        EditorGUI::Drag("SHADERCLASS", GarinIO::GetWithAfterAssetDir(complete_path));
    }
    else if (extension == ".lua")
    {
        EditorGUI::Drag("SCRIPTCLASSLUA", GarinIO::GetWithAfterAssetDir(complete_path));
    }
}