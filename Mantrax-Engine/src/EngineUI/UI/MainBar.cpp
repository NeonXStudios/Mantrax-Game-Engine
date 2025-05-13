#include <MainBar.h>
#include <IconsManager.h>
#include <EngineUI.h>
#include <UIStyle.h>
#include <SceneData.h>
#include <CompilerView.h>
#include <Ensambler.h>

#include <imgui_internal.h>
#include <UIMasterDrawer.h>
#include <SceneManager.h>
#include <FileManager.h>
#include <EditorGUI.h>
#include <ServiceLocator.h>

void MainBar::on_draw()
{
    EngineUI *editor_ui = ServiceLocator::get<EngineUI>().get();
    SceneManager *sceneM = ServiceLocator::get<SceneManager>().get();

    ShowNewScenePopup();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 8.0f));
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
                    std::string bg_pick_path = editor_ui->configs->current_proyect + "/gb.jpg";
                    GarinIO::screenshot(Gfx::main_render->get_render(), 1920, 1080, bg_pick_path.c_str());
                    SceneData::save_scene(sceneM->get_current_scene());
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
                    editor_ui->configs->project_select = false;
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Game"))
        {
            if (ImGui::MenuItem("Input"))
            {
                // ...
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
            if (ImGui::MenuItem("Material Maker"))
            {
                UIMasterDrawer::get_instance().get_component<MaterialMaker>()->is_open = true;
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
                editor_ui->configs->current_theme = "DefaultClassic";
            }
            if (ImGui::MenuItem("Default Light"))
            {
                UIStyle::SetStyleUI(DefaultWhite);
                editor_ui->configs->current_theme = "DefaultWhite";
            }
            if (ImGui::MenuItem("Default Dark"))
            {
                UIStyle::SetStyleUI(DefaultDark);
                editor_ui->configs->current_theme = "DefaultDark";
            }
            if (ImGui::MenuItem("Darkness"))
            {
                UIStyle::SetStyleUI(Darkness);
                editor_ui->configs->current_theme = "Darkness";
            }
            if (ImGui::MenuItem("Dracula"))
            {
                UIStyle::SetStyleUI(Dracula);
                editor_ui->configs->current_theme = "Dracula";
            }
            if (ImGui::MenuItem("RedDark"))
            {
                UIStyle::SetStyleUI(RedDark);
                editor_ui->configs->current_theme = "RedDark";
            }
            if (ImGui::MenuItem("Dark"))
            {
                UIStyle::SetStyleUI(Dark);
                editor_ui->configs->current_theme = "Dark";
            }
            if (ImGui::MenuItem("Grey"))
            {
                UIStyle::SetStyleUI(Grey);
                editor_ui->configs->current_theme = "Grey";
            }
            if (ImGui::MenuItem("WhiteY"))
            {
                UIStyle::SetStyleUI(WhiteY);
                editor_ui->configs->current_theme = "WhiteY";
            }
            if (ImGui::MenuItem("DarkGrey"))
            {
                UIStyle::SetStyleUI(DarkGrey);
                editor_ui->configs->current_theme = "DarkGrey";
            }
            if (ImGui::MenuItem("Black"))
            {
                UIStyle::SetStyleUI(Black);
                editor_ui->configs->current_theme = "Black";
            }
            if (ImGui::MenuItem("DarkRounded"))
            {
                UIStyle::SetStyleUI(DarkRounded);
                editor_ui->configs->current_theme = "DarkRounded";
            }
            ImGui::EndMenu();
        }

        ImVec2 buttonSize(23, 23);

        ImVec2 windowSize = ImGui::GetWindowSize();

        float centerX = (windowSize.x - buttonSize.x) * 0.5f;
        float centerY = (windowSize.y - buttonSize.y) * 0.5f;

        ImGui::SetCursorPos(ImVec2(centerX, centerY));

        unsigned int current_icon = AppSettings::is_playing ? IconsManager::PAUSE() : IconsManager::PLAY();

        if (ImGui::ImageButton(
                (void *)(intptr_t)current_icon,
                buttonSize,
                ImVec2(0, 0),
                ImVec2(1, 1),
                0,
                ImVec4(0, 0, 0, 0),
                ImVec4(1, 1, 1, 1)))
        {
            std::string command = ".\\data\\PlayBackEngine\\Mantrax_PlayBackEngine.exe";
            std::string parameter = editor_ui->configs->current_proyect;
            std::string fullCommand = command + " " + parameter;

            std::string newWorkDir = FileManager::get_execute_path();
            std::wstring wideWorkingDir(newWorkDir.begin(), newWorkDir.end());
            if (!SetCurrentDirectoryW(wideWorkingDir.c_str()))
            {
                std::cerr << "Error al cambiar directorio para lanzar PlayBackEngine." << std::endl;
            }
            int result_execution = system(fullCommand.c_str());
            std::cout << "Entering play mode" << std::endl;
        }

        ImGui::EndMainMenuBar();
    }
    ImGui::PopStyleVar();
}

void MainBar::ShowNewScenePopup()
{
    SceneManager *sceneM = ServiceLocator::get<SceneManager>().get();

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
            sceneM->load_scene(new_scene_name);
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
}