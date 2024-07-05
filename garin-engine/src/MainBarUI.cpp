#include "../includes/MainBarUI.h"
#include "../includes/WindowsCompiler.h"
#include "../includes/UIAdministrator.h"

#include <GarinGraphics.h>
#include <GarinEvents.h>
#include <GarinComponents.h>
#include <GarinBehaviours.h>

void MainBarUI::draw(Entity *owner)
{
    ShowNewScenePopup();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 32 / 2));

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
                if (!AppSettings::is_playing)
                {
                    Graphics::get_current_scene()->save_scene();
                }
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

                // create_directory_if_not_exists(build_path);
                // create_directory_if_not_exists(build_path + "scenes/");
                // create_directory_if_not_exists(build_path + "packages/");
                // create_directory_if_not_exists(build_path + "packages/shaders/");
                // create_directory_if_not_exists(build_path + "packages/models/");
                // create_directory_if_not_exists(build_path + "packages/audios/");
                // create_directory_if_not_exists(data_path);

                std::thread compile_thread(WindowsCompiler::compile_windows);

                compile_thread.join();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Engine"))
        {
            if (ImGui::MenuItem("Recompile Scripts"))
            {
                GameBehaviourFactory::instance().reload_components();
                Graphics::graphics->engine_libs_loader.get()->load_components();
            }
            ImGui::EndMenu();
        }
    }

    ImGui::SameLine((ImGui::GetWindowWidth() / 2) - 16);

    if (ImGui::ImageButton((void *)(intptr_t)IconsManager::PLAY(), ImVec2(16, 16)))
    {

        AppSettings::is_playing = !AppSettings::is_playing;

        if (AppSettings::is_playing)
        {
            Graphics::get_current_scene()->save_scene();
            Graphics::get_current_scene()->load_scene(Graphics::get_current_scene()->scene_name);
            DebugGame::add_message("Entering in playmode", DebugGame::logger);
        }
        else
        {
            Graphics::get_current_scene()->load_scene(Graphics::get_current_scene()->scene_name);
            DebugGame::add_message("Stopping in playmode", DebugGame::logger);
        }

        std::cout << "Entering play mode" << std::endl;
    }

    ImGui::EndMainMenuBar();
    ImGui::PopStyleVar();
}
