#include <FileBar.h>
#include <EngineUI.h>

void FileBar::on_draw()
{
    std::string new_name = "Classes##" + std::to_string(window_id);

    ImGui::Begin(new_name.c_str(), & is_open);
    ShowDirectoryTree(FileManager::get_game_path() + "/clscpp");

    if (ImGui::BeginPopupContextWindow("AssetsPopup", ImGuiMouseButton_Right))
    {
        if (ImGui::MenuItem("Create Class"))
        {
            show_script_popup = true;
        }

        ImGui::EndPopup();
    }

    if (ImGui::Button("Open Class Proyect"))
    {
        std::filesystem::current_path(FileManager::get_game_path() + "/clscpp");

        int result = system("code .");
    }

    ImGui::End();

    if (show_script_popup)
    {
        ImGui::OpenPopup("Create Script");
    }

    if (ImGui::BeginPopupModal("Create Script", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Class Name:");
        ImGui::InputText("##scriptname", script_name, IM_ARRAYSIZE(script_name));

        if (ImGui::Button("Create"))
        {
            create_script_files(script_name);
            show_script_popup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            show_script_popup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void FileBar::ShowDirectoryTree(const std::filesystem::path &path)
{
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            if (ImGui::TreeNode(entry.path().filename().string().c_str()))
            {
                if (entry.path().extension().string() != ".cpp")
                {
                    if (ImGui::Selectable(GarinIO::GetFileNameWithoutExtension(entry.path().filename().string()).c_str(), selected_item == entry.path().string()))
                    {
                        selected_item = entry.path().string();
                        // std::cout << "Directorio seleccionado: " << selected_item << std::endl;
                    }

                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                    {
                        OpenAssociatedFiles(entry.path());
                    }

                    EditorGUI::Drag("SCRIPTCLASS", GarinIO::GetFileNameWithoutExtension(entry.path().filename().string()));
                    ShowDirectoryTree(entry.path());
                    ImGui::TreePop();
                }
            }
        }
        else
        {
            if (entry.path().extension().string() != ".cpp")
            {
                if (ImGui::Selectable(GarinIO::GetFileNameWithoutExtension(entry.path().filename().string()).c_str(), selected_item == entry.path().string()))
                {
                    selected_item = entry.path().string();
                    // std::cout << "Archivo seleccionado: " << selected_item << std::endl;
                }

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                    OpenAssociatedFiles(entry.path());
                }

                EditorGUI::Drag("SCRIPTCLASS", GarinIO::GetFileNameWithoutExtension(entry.path().filename().string()));
            }
        }
    }
}

void FileBar::OpenAssociatedFiles(const std::filesystem::path &path)
{
    try
    {
        std::filesystem::path header_file = path.parent_path() / (path.stem().string() + ".h");
        std::filesystem::path cpp_file = path.parent_path() / (path.stem().string() + ".cpp");

        std::string command = "code";

        if (std::filesystem::exists(header_file))
        {
            std::cout << "Abriendo archivo de cabecera: " << header_file << std::endl;
            command += " \"" + header_file.string() + "\"";
        }

        if (std::filesystem::exists(cpp_file))
        {
            std::cout << "Abriendo archivo de implementación: " << cpp_file << std::endl;
            command += " \"" + cpp_file.string() + "\"";
        }

        if (std::system(command.c_str()) != 0)
        {
            std::cerr << "Error al abrir archivos en VSCode." << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void FileBar::create_script_files(const std::string &script_name)
{
    std::string header_file = FileManager::get_game_path() + "assets/clscpp/" + script_name + ".h";
    std::string source_file = FileManager::get_game_path() + "assets/clscpp/" + script_name + ".cpp";

    // Crear archivo .h
    std::ofstream header(header_file);
    if (header.is_open())
    {
        header << "#pragma once\n";
        header << "#include <GarinNatives.h>\n\n";
        header << "#include <GarinGraphics.h>\n\n";
        header << "#include <GarinMaths.h>\n\n";
        header << "#include <GarinComponents.h>\n\n";
        header << "\n";
        header << "class " << script_name << " : public GameBehaviour\n";
        header << "{\n";
        header << "public:\n";
        header << "    void on_init() override;\n";
        header << "    void on_tick() override;\n";
        header << "};\n";
        header << "\n";
        header << "GARINCLASS(" + script_name + ")";
        header.close();
    }
    else
    {
        std::cerr << "Error al crear el archivo de cabecera: " << header_file << std::endl;
        return;
    }

    // Crear archivo .cpp
    std::ofstream source(source_file);
    if (source.is_open())
    {
        source << "#include \"" << script_name << ".h\"\n\n";
        source << "\n";
        source << "void " << script_name << "::on_init()\n";
        source << "{\n";
        source << "    // Inicialización\n";
        source << "}\n\n";
        source << "void " << script_name << "::on_tick()\n";
        source << "{\n";
        source << "    // Lógica de actualización\n";
        source << "}\n";
        source.close();
    }
    else
    {
        std::cerr << "Error al crear el archivo de implementación: " << source_file << std::endl;
        return;
    }
}
