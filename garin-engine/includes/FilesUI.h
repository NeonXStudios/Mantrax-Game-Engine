#pragma once
#include "UIComponent.h"

class FilesUI : public UIComponent
{
public:
    bool show_script_popup = false;
    char script_name[128] = "";
    std::string selected_item;
    EditorConfigs *configs;
    void draw(Entity *owner, EditorConfigs *configs) override;

    void ShowDirectoryTree(const std::filesystem::path &path)
    {
        for (const auto &entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                if (ImGui::TreeNode(entry.path().filename().string().c_str()))
                {
                    // Hacer el directorio seleccionable
                    if (ImGui::Selectable(entry.path().filename().string().c_str(), selected_item == entry.path().string()))
                    {
                        selected_item = entry.path().string();
                        // Log en la consola
                        std::cout << "Directorio seleccionado: " << selected_item << std::endl;
                    }

                    ShowDirectoryTree(entry.path());
                    ImGui::TreePop();
                }
            }
            else
            {
                // Hacer el archivo seleccionable
                if (ImGui::Selectable(entry.path().filename().string().c_str(), selected_item == entry.path().string()))
                {
                    selected_item = entry.path().string();
                    // Log en la consola
                    std::cout << "Archivo seleccionado: " << selected_item << std::endl;
                }
            }
        }
    }

    void create_script_files(const std::string &script_name)
    {
        std::string header_file = FileManager::get_execute_path() + configs->current_proyect + "/clscpp/" + script_name + ".h";
        std::string source_file = FileManager::get_execute_path() + configs->current_proyect + "/clscpp/" + script_name + ".cpp";

        // Crear archivo .h
        std::ofstream header(header_file);
        if (header.is_open())
        {
            header << "#pragma once\n";
            header << "#include <GarinCore.h>\n\n";
            header << "class " << script_name << " : public GameBehaviour\n";
            header << "{\n";
            header << "public:\n";
            header << "    void on_init() override;\n";
            header << "    void on_tick() override;\n";
            header << "};\n";
            header << "\n";
            header << "REGISTER_GAMEBEHAVIOUR_CLASS(" + script_name + ")";
            header.close();
        }

        // Crear archivo .cpp
        std::ofstream source(source_file);
        if (source.is_open())
        {
            source << "#include \"" << script_name << ".h\"\n\n";
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
    }
};