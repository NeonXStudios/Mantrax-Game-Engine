#include "../includes/MainBarUI.h"
#include "../includes/WindowsCompiler.h"
#include "../includes/UIAdministrator.h"

#include <GarinGraphics.h>
#include <GarinEvents.h>
#include <GarinComponents.h>
#include <GarinBehaviours.h>
#include <UIStyle.h>

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
                    SceneManager::GetOpenScene()->save_scene();
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
                std::thread hilo([]()
                                 { 
                                    UINotification::AddNotification("Recompiling libraries...", 10.0f);

    GameBehaviourFactory::instance().reload_components();

    UINotification::AddNotification("Compiling libraries (wait for it)...", 10.0f);

    std::string cmake_path = FileManager::get_project_path() + "wlibsgpp/";

    // Formar los comandos
    std::string cmake_command = "cd /d " + cmake_path + " && cmake -G \"Visual Studio 17 2022\" .";
    std::string msbuild_command = "cd /d " + cmake_path + " && msbuild GarinGameCore.sln /p:Configuration=Debug";

    // Ejecutar el comando CMake
    int result = system(cmake_command.c_str());

    if (result == 0)
    {
        UINotification::AddNotification("CMake Reloaded...", 10.0f);
        UINotification::AddNotification("Starting compilation of the libraries (Wait)...", 10.0f);

        // Ejecutar el comando MSBuild
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

    if (Graphics::graphics != nullptr)
    {
        if (Graphics::graphics->engine_libs_loader.get() != nullptr)
        {
            std::cout << "Starting reload components" << std::endl;

            Graphics::graphics->engine_libs_loader.get()->load_components();

            UINotification::AddNotification("Compiled and reloaded libraries...", 3.0f);
        }
    }
    else
    {
        std::cout << "Graphics info never assigned" << std::endl;
    } });
                hilo.join();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Engine Themes"))
        {
            if (ImGui::MenuItem("Default Classic"))
            {
                UIStyle::SetStyleUI(DefaultClassic);
            }
            if (ImGui::MenuItem("Default Light"))
            {
                UIStyle::SetStyleUI(DefaultWhite);
            }
            if (ImGui::MenuItem("Default Dark"))
            {
                UIStyle::SetStyleUI(DefaultDark);
            }
            if (ImGui::MenuItem("Darkness"))
            {
                UIStyle::SetStyleUI(Darkness);
            }
            if (ImGui::MenuItem("Dracula"))
            {
                UIStyle::SetStyleUI(Dracula);
            }
            if (ImGui::MenuItem("RedDark"))
            {
                UIStyle::SetStyleUI(RedDark);
            }
            if (ImGui::MenuItem("Dark"))
            {
                UIStyle::SetStyleUI(Dark);
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
            SceneManager::GetOpenScene()->save_scene();
            SceneManager::GetOpenScene()->load_scene(SceneManager::GetOpenScene()->scene_name);
            DebugGame::add_message("Entering in playmode", DebugGame::logger);
        }
        else
        {
            SceneManager::GetOpenScene()->load_scene(SceneManager::GetOpenScene()->scene_name);
            DebugGame::add_message("Stopping in playmode", DebugGame::logger);
        }

        std::cout << "Entering play mode" << std::endl;
    }

    ImGui::EndMainMenuBar();
    ImGui::PopStyleVar();
}

void MainBarUI::recompileAndReloadLibraries()
{
    UINotification::AddNotification("Recompiling libraries...", 10.0f);

    GameBehaviourFactory::instance().reload_components();

    UINotification::AddNotification("Compiling libraries (wait for it)...", 10.0f);

    std::string cmake_path = FileManager::get_project_path() + "wlibsgpp/";

    // Formar los comandos
    std::string cmake_command = "cd /d " + cmake_path + " && cmake -G \"Visual Studio 17 2022\" .";
    std::string msbuild_command = "cd /d " + cmake_path + " && msbuild GarinGameCore.sln /p:Configuration=Debug";

    // Ejecutar el comando CMake
    int result = system(cmake_command.c_str());

    if (result == 0)
    {
        UINotification::AddNotification("CMake Reloaded...", 10.0f);
        UINotification::AddNotification("Starting compilation of the libraries (Wait)...", 10.0f);

        // Ejecutar el comando MSBuild
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

    if (Graphics::graphics != nullptr)
    {
        if (Graphics::graphics->engine_libs_loader.get() != nullptr)
        {
            std::cout << "Starting reload components" << std::endl;

            Graphics::graphics->engine_libs_loader.get()->load_components();

            UINotification::AddNotification("Compiled and reloaded libraries...", 3.0f);
        }
    }
    else
    {
        std::cout << "Graphics info never assigned" << std::endl;
    }
}
