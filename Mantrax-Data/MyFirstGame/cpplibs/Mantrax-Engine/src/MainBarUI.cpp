#include "../includes/MainBarUI.h"
#include "../includes/WindowsCompiler.h"
#include "../includes/UIAdministrator.h"

#include <GarinGraphics.h>
#include <GarinEvents.h>
#include <GarinComponents.h>
#include <GarinBehaviours.h>
#include <UIStyle.h>
#include <SceneData.h>
#include <filesystem>
#include <GraphicsManagerUI.h>

namespace fs = std::filesystem;

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
                    std::string bg_pick_path = configs->current_proyect + "/gb.jpg";
                    GarinIO::screenshot(Gfx::main_render->get_render(), 1920, 1080, bg_pick_path.c_str());

                    SceneData::save_scene();
                }
            }

            if (ImGui::MenuItem("Close Proyect"))
            {
                if (!AppSettings::is_playing)
                {
                    configs->project_select = false;
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Game"))
        {

            if (ImGui::MenuItem("Input"))
            {
            }

            if (ImGui::MenuItem("UI"))
            {
                game->uieditor->makerui->is_open = true;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows"))
        {

            if (ImGui::MenuItem("Scene"))
            {
                game->uieditor->sceneui->is_open = true;
            }

            if (ImGui::MenuItem("Game Settings"))
            {
                game->uieditor->settingsui->is_open = true;
            }

            if (ImGui::MenuItem("Inspector"))
            {
                game->uieditor->inspector->is_open = true;
            }

            if (ImGui::MenuItem("Hierarchy"))
            {
                game->uieditor->hierarchyui->is_open = true;
            }

            if (ImGui::MenuItem("Assets"))
            {
                game->uieditor->assetsui->is_open = true;
            }

            if (ImGui::MenuItem("Scripts"))
            {
                game->uieditor->filesui->is_open = true;
            }

            if (ImGui::MenuItem("Input System"))
            {
                game->uieditor->inputui->is_open = true;
            }

            if (ImGui::MenuItem("Graphics Settings"))
            {
                game->uieditor->graphicsui->is_open = true;
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
                std::string directoryPath = FileManager::get_project_path() + "clscpp/";
                std::string outputFilePath = FileManager::get_project_path() + "wlibsgpp/GameLibsLoader.h";

                std::ofstream clearFile(outputFilePath, std::ofstream::out | std::ofstream::trunc);
                clearFile.close();

                process_header_files(directoryPath, outputFilePath);

                std::string register_core = FileManager::get_project_path() + "wlibsgpp/GarinGameCore.cpp";
                for (const auto &entry : fs::directory_iterator(directoryPath))
                {
                    if (entry.path().extension() == ".h")
                    {
                        std::cout << "INSERTANDO ARCHIVO: " << entry.path().filename().stem().string() << std::endl;
                        insert_register(register_core, entry.path().filename().stem().string());
                    }
                }

                UINotification::AddNotification("Recompiling libraries...", 10.0f);

                GameBehaviourFactory::instance().reload_components();

                UINotification::AddNotification("Compiling libraries (wait for it)...", 10.0f);

                std::string cmake_path = FileManager::get_project_path() + "wlibsgpp/Compiler-Lib/GarinEditorEngine/";

                // Formar los comandos
                std::string cmake_command = "cd /d " + cmake_path + " && cmake -G \"Visual Studio 17 2022\" .";
                std::string msbuild_command = "cd /d " + cmake_path + " && msbuild GarinEditor.sln /p:Configuration=Debug";

                // Ejecutar el comando CMake
                int result = system(cmake_command.c_str());

                if (result == 0)
                {
                    UINotification::AddNotification("CMake Reloaded...", 10.0f);
                    UINotification::AddNotification("Starting compilation of the libraries (Wait)...", 10.0f);

                    int result_build = system(msbuild_command.c_str());

                    if (result_build == 0)
                    {
                        UINotification::AddNotification("Successfully compiled libraries...", 10.0f);
                    }
                    else
                    {
                        UINotification::AddNotification("Error during the compilation of the libraries...", 10.0f);
                        std::cerr << "Error during msbuild execution, result code: " << result_build << std::endl;
                    }
                }
                else
                {
                    UINotification::AddNotification("Error during CMake execution...", 10.0f);
                    std::cerr << "Error during cmake execution, result code: " << result << std::endl;
                }

                DynamicLibLoader::instance->load_components();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Engine Themes"))
        {
            if (ImGui::MenuItem("Default Classic"))
            {
                UIStyle::SetStyleUI(DefaultClassic);
                configs->current_theme = "DefaultClassic";
            }
            if (ImGui::MenuItem("Default Light"))
            {
                UIStyle::SetStyleUI(DefaultWhite);
                configs->current_theme = "DefaultWhite";
            }
            if (ImGui::MenuItem("Default Dark"))
            {
                UIStyle::SetStyleUI(DefaultDark);
                configs->current_theme = "DefaultDark";
            }
            if (ImGui::MenuItem("Darkness"))
            {
                UIStyle::SetStyleUI(Darkness);
                configs->current_theme = "Darkness";
            }
            if (ImGui::MenuItem("Dracula"))
            {
                UIStyle::SetStyleUI(Dracula);
                configs->current_theme = "Dracula";
            }
            if (ImGui::MenuItem("RedDark"))
            {
                UIStyle::SetStyleUI(RedDark);
                configs->current_theme = "RedDark";
            }
            if (ImGui::MenuItem("Dark"))
            {
                UIStyle::SetStyleUI(Dark);
                configs->current_theme = "Dark";
            }
            ImGui::EndMenu();
        }
    }

    ImGui::SameLine((ImGui::GetWindowWidth() / 2) - 16);

    unsigned int current_icon = AppSettings::is_playing ? IconsManager::PAUSE() : IconsManager::PLAY();

    if (ImGui::ImageButton((void *)(intptr_t)current_icon, ImVec2(16, 16)))
    {
        AppSettings::is_playing = !AppSettings::is_playing;

        if (AppSettings::is_playing)
        {
            Entity *selectobj = owner;

            if (selectobj != nullptr)
            {
                selectobj = owner;
            }

            SceneData::save_scene();
            SceneData::load_scene(SceneManager::GetOpenScene()->scene_name);

            if (selectobj != nullptr)
                game->set_object_select(selectobj);

            std::string command = ".\\data\\PlayBackEngine\\Mantrax_PlayBackEngine.exe";
            std::string parameter = configs->current_proyect;

            std::string fullCommand = command + " " + parameter;

            std::string newWorkDir = FileManager::get_execute_path();
            std::wstring wideWorkingDir(newWorkDir.begin(), newWorkDir.end());

            if (!SetCurrentDirectoryW(wideWorkingDir.c_str())){
                std::cerr << "Error on try create Play Back Engine" << std::endl;
            }

            int result_execution = system(fullCommand.c_str());
        }
        else
        {
            SceneData::load_scene(SceneManager::GetOpenScene()->scene_name);
        }

        std::cout << "Entering play mode" << std::endl;
    }

    ImGui::EndMainMenuBar();
    ImGui::PopStyleVar();

    ImGui::Begin("Scene Settings");
    gamescene::getInstance().configs->camera_speed_sens = EditorGUI::Float("Camera sensitivity", gamescene::getInstance().configs->camera_speed_sens);
    ImGui::End();
}

void MainBarUI::recompileAndReloadLibraries()
{
}
