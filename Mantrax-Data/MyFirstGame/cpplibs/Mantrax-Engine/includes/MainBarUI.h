#pragma once
#include "UIComponent.h"
#include "../includes/UINotification.h"
#include <GarinNatives.h>
#include <SceneData.h>
#include <Core.h>
#include "gamescene.h"
#include <EditorConfigs.h>

class gamescene;
class GARINLIBS_API MainBarUI : public UIComponent
{
public:
    gamescene *game;
    bool show_new_scene_popup = false;
    char new_scene_name[128] = "";
    EditorConfigs *configs;

    void draw(Entity *select_obj) override;

    void process_header_files(const std::string &directoryPath, const std::string &outputFilePath)
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

    void ShowNewScenePopup()
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

    void insert_register(const std::string &filePath, const std::string &lineToInsert)
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

    void recompileAndReloadLibraries();
};