#include <MainBar.h>
#include <IconsManager.h>
#include <EngineUI.h>
#include <UIStyle.h>
#include <SceneData.h>

void MainBar::on_draw()
{
    ShowNewScenePopup();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 32 / 2));

    if (ImGui::BeginMainMenuBar())
    {
        ImGui::Image((void *)(intptr_t)IconsManager::ENGINE_LOGO(), ImVec2(32, 28));
        ImGui::SameLine();

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
                    std::string bg_pick_path = EngineUI::getInstance().configs->current_proyect + "/gb.jpg";
                    GarinIO::screenshot(Gfx::main_render->get_render(), 1920, 1080, bg_pick_path.c_str());

                    SceneData::save_scene();
                }
            }

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

            if (ImGui::MenuItem("Scene"))
            {
                // OPENED UIS
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
                        std::cout << "MAKING H CLASS GETTER: " << entry.path().filename().stem().string() << std::endl;
                        insert_register(register_core, entry.path().filename().stem().string());
                    }
                }

                UINotification::AddNotification("Recompiling libraries...", 10.0f);

                // GameBehaviourFactory::instance().reload_components();

                UINotification::AddNotification("Compiling libraries (wait for it)...", 10.0f);

                std::string cmake_path = FileManager::get_project_path() + "wlibsgpp/";

                std::string cmake_command =
                    "cd /d " + cmake_path +
                    " && if not exist bin mkdir bin && cd bin && cmake ..";

                std::string msbuild_command = "cd /d " + cmake_path + " && cd bin && msbuild MantraxRuntimeCore.sln /p:Configuration=Debug";

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

                // DynamicLibLoader::instance->load_components();
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

        // AppSettings::is_playing = !AppSettings::is_playing;

        if (AppSettings::is_playing)
        {
            // Entity *selectobj = owner;

            // if (selectobj != nullptr)
            // {
            //     selectobj = owner;
            // }

            // SceneData::save_scene();
            // SceneData::load_scene(SceneManager::GetOpenScene()->scene_name);

            // if (selectobj != nullptr)
            //     game->set_object_select(selectobj);
        }
        // else
        // {
        //     SceneData::load_scene(SceneManager::GetOpenScene()->scene_name);
        // }

        std::cout << "Entering play mode" << std::endl;
    }

    ImGui::EndMainMenuBar();
    ImGui::PopStyleVar();

    ImGui::Begin("Scene Settings");
    EngineUI::getInstance().configs->camera_speed_sens = EditorGUI::Float("Camera sensitivity", EngineUI::getInstance().configs->camera_speed_sens);
    ImGui::End();
}

void MainBar::process_header_files(const std::string &directoryPath, const std::string &outputFilePath)
{
    std::ofstream outputFile(outputFilePath);

    for (const auto &entry : fs::directory_iterator(directoryPath))
    {
        if (entry.path().extension() == ".h")
        {
            outputFile << "#include <" << entry.path().filename().string() << ">" << std::endl;
        }
    }

    outputFile.close();
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
}

void MainBar::insert_register(const std::string &filePath, const std::string &lineToInsert)
{
    std::ifstream inFile(filePath);
    std::stringstream buffer;

    if (!inFile.is_open())
    {
        std::cerr << "Error: No se pudo abrir el archivo de entrada." << std::endl;
        return;
    }

    // Leer el contenido del archivo y buscar las líneas de inserción
    std::string line;
    bool foundFinalRegisterData = false;

    while (std::getline(inFile, line))
    {
        if (line.find("/////////_FINALREGISTERDATA_/////////") != std::string::npos)
        {
            buffer << "GCLASSDLL(" << lineToInsert << ");" << std::endl; // Insertar las líneas nuevas antes de la marca final
            foundFinalRegisterData = true;
        }
        buffer << line << std::endl;
    }

    inFile.close();

    if (!foundFinalRegisterData)
    {
        std::cerr << "Error: No se encontró la marca final en el archivo." << std::endl;
        return;
    }

    // Escribir el contenido modificado de vuelta al archivo
    std::ofstream outFile(filePath);
    if (!outFile.is_open())
    {
        std::cerr << "Error: No se pudo abrir el archivo de salida para escritura." << std::endl;
        return;
    }

    outFile << buffer.str();
    outFile.close();

    std::cout << "Se agregó la línea correctamente antes de FINALREGISTERDATA." << std::endl;
}