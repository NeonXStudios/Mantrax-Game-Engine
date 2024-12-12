#include <MainBar.h>
#include <IconsManager.h>
#include <EngineUI.h>
#include <UIStyle.h>
#include <SceneData.h>
#include <CompilerView.h>
#include <Ensambler.h>

void MainBar::on_draw()
{
    ShowNewScenePopup();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 32 / 2));

    if (ImGui::BeginMainMenuBar())
    {
        ImGui::Image((void *)(intptr_t)IconsManager::ENGINE_LOGO(), ImVec2(32, 28));
        ImGui::SameLine();

        if (ImGui::BeginMenu("File", ImGuiWindowFlags_NoDecoration))
        {
            if (ImGui::MenuItem("New Scene"))
            {
                show_new_scene_popup = true;
            }

            if (ImGui::MenuItem("Save"))
            {
                if (!AppSettings::is_playing)
                {
                    std::string bg_pick_path = EngineUI::getInstance().configs->current_proyect + "/gb.jpg";
                    GarinIO::screenshot(Gfx::main_render->get_render(), 1920, 1080, bg_pick_path.c_str());

                    SceneData::save_scene();
                }
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Build"))
            {
                UIMasterDrawer::get_instance().get_component<CompilerView>()->is_open = true;
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Close Proyect"))
            {
                if (!AppSettings::is_playing)
                {
                    EngineUI::getInstance().configs->project_select = false;
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Game"))
        {

            if (ImGui::MenuItem("Input"))
            {
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows"))
        {

            if (ImGui::MenuItem("Hierarchy"))
            {
                UIMasterDrawer::get_instance().get_component<Hierarchy>()->is_open = true;
            }

            if (ImGui::MenuItem("FileBar"))
            {
                UIMasterDrawer::get_instance().get_component<FileBar>()->is_open = true;
            }

            if (ImGui::MenuItem("AssetsFiles"))
            {
                UIMasterDrawer::get_instance().get_component<AssetsFiles>()->is_open = true;
            }

            if (ImGui::MenuItem("Inspector"))
            {
                UIMasterDrawer::get_instance().get_component<Inspector>()->is_open = true;
            }

            if (ImGui::MenuItem("SceneView"))
            {
                UIMasterDrawer::get_instance().get_component<SceneView>()->is_open = true;
            }

            if (ImGui::MenuItem("EngineSettings"))
            {
                UIMasterDrawer::get_instance().get_component<EngineSettings>()->is_open = true;
            }
            if (ImGui::MenuItem("AnimatorView"))
            {
                UIMasterDrawer::get_instance().get_component<AnimatorView>()->is_open = true;
            }

            if (ImGui::MenuItem("UIEditor"))
            {
                UIMasterDrawer::get_instance().get_component<UIEditor>()->is_open = true;
            }

            if (ImGui::MenuItem("CompilerView"))
            {
                UIMasterDrawer::get_instance().get_component<CompilerView>()->is_open = true;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Engine"))
        {
            if (ImGui::MenuItem("Recompile Scripts"))
            {
                std::thread compile_thread(Ensambler::start_scripts_reload);
                compile_thread.detach();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Engine Themes"))
        {
            if (ImGui::MenuItem("Default Classic"))
            {
                UIStyle::SetStyleUI(DefaultClassic);
                EngineUI::getInstance().configs->current_theme = "DefaultClassic";
            }
            if (ImGui::MenuItem("Default Light"))
            {
                UIStyle::SetStyleUI(DefaultWhite);
                EngineUI::getInstance().configs->current_theme = "DefaultWhite";
            }
            if (ImGui::MenuItem("Default Dark"))
            {
                UIStyle::SetStyleUI(DefaultDark);
                EngineUI::getInstance().configs->current_theme = "DefaultDark";
            }
            if (ImGui::MenuItem("Darkness"))
            {
                UIStyle::SetStyleUI(Darkness);
                EngineUI::getInstance().configs->current_theme = "Darkness";
            }
            if (ImGui::MenuItem("Dracula"))
            {
                UIStyle::SetStyleUI(Dracula);
                EngineUI::getInstance().configs->current_theme = "Dracula";
            }
            if (ImGui::MenuItem("RedDark"))
            {
                UIStyle::SetStyleUI(RedDark);
                EngineUI::getInstance().configs->current_theme = "RedDark";
            }
            if (ImGui::MenuItem("Dark"))
            {
                UIStyle::SetStyleUI(Dark);
                EngineUI::getInstance().configs->current_theme = "Dark";
            }
            ImGui::EndMenu();
        }
    }

    ImGui::SameLine((ImGui::GetWindowWidth() / 2) - 16);

    unsigned int current_icon = AppSettings::is_playing ? IconsManager::PAUSE() : IconsManager::PLAY();

    if (ImGui::ImageButton((void *)(intptr_t)current_icon, ImVec2(16, 16)))
    {
        std::string command = ".\\data\\PlayBackEngine\\Mantrax_PlayBackEngine.exe";
        std::string parameter = EngineUI::getInstance().configs->current_proyect;

        std::string fullCommand = command + " " + parameter;

        std::string newWorkDir = FileManager::get_execute_path();
        std::wstring wideWorkingDir(newWorkDir.begin(), newWorkDir.end());

        if (!SetCurrentDirectoryW(wideWorkingDir.c_str()))
        {
            std::cerr << "Error on try create Play Back Engine" << std::endl;
        }

        int result_execution = system(fullCommand.c_str());

        std::cout << "Entering play mode" << std::endl;
    }

    ImGui::EndMainMenuBar();
    ImGui::PopStyleVar();

    ImGui::Begin("Scene Settings");
    EngineUI::getInstance().configs->camera_speed_sens = EditorGUI::Float("Camera sensitivity", EngineUI::getInstance().configs->camera_speed_sens);
    ImGui::End();
}

void MainBar::ShowNewScenePopup()
{
    if (show_new_scene_popup)
    {
        ImGui::OpenPopup("New Scene");
    }

    if (ImGui::BeginPopupModal("New Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Enter the name of the new scene:");
        ImGui::InputText("##NewSceneName", new_scene_name, IM_ARRAYSIZE(new_scene_name));

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            // Load the new scene
            SceneData::load_scene(new_scene_name);
            show_new_scene_popup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            show_new_scene_popup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImVec2 main_window_size = ImGui::GetIO().DisplaySize;

    float bar_height = 25.0f;

    ImGui::SetNextWindowPos(ImVec2(0, main_window_size.y - bar_height));
    ImGui::SetNextWindowSize(ImVec2(main_window_size.x, bar_height));

    ImGui::Begin("Bottom Bar", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Información: Todo está funcionando correctamente.");
    ImGui::SameLine();
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    ImGui::End();
}
