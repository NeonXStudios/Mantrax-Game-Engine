#include "../includes/MainBarUI.h"
#include "../includes/WindowsCompiler.h"
#include "../includes/UIAdministrator.h"

#include <GarinGraphics.h>

void MainBarUI::draw(Entity *owner)
{
    ShowNewScenePopup();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Scene"))
            {
                show_new_scene_popup = true;
            }

            if (ImGui::MenuItem("Save"))
            {
                Graphics::get_current_scene()->save_scene();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Build"))
        {
            if (ImGui::MenuItem("Windows"))
            {
                std::string build_path = FileManager::get_execute_path() + "/projects/test/build/";
                std::string data_path = FileManager::get_execute_path() + "/projects/test/wlibsgpp/";

                auto create_directory_if_not_exists = [](const std::string &path)
                {
                    if (!std::filesystem::exists(path))
                    {
                        if (std::filesystem::create_directory(path))
                        {
                            std::cout << "Folder correctly created: " << path << std::endl;
                        }
                        else
                        {
                            std::cerr << "Failed to create folder: " << path << std::endl;
                        }
                    }
                    else
                    {
                        std::cout << "Folder already exists: " << path << std::endl;
                    }
                };

                create_directory_if_not_exists(build_path);
                create_directory_if_not_exists(build_path + "scenes/");
                create_directory_if_not_exists(build_path + "packages/");
                create_directory_if_not_exists(build_path + "packages/shaders/");
                create_directory_if_not_exists(build_path + "packages/models/");
                create_directory_if_not_exists(build_path + "packages/audios/");
                create_directory_if_not_exists(data_path);

                WindowsCompiler::compile_windows();
            }
            ImGui::EndMenu();
        }
    }

    ImGui::EndMainMenuBar();
}
