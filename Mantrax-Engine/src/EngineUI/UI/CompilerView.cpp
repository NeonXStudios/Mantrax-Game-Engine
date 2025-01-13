#include <CompilerView.h>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <imgui.h>
#include <stdio.h>
#include <WindowsCompiler.h>

namespace fs = std::filesystem;

// Variables estáticas para el UI
static char gameName[128] = "MyGame";
static char gameImagePath[256] = "";
static int renderOption = 0; // 0=OpenGL, 1=DirectX, 2=Vulkan, 3=Metal
static bool scene1 = true, scene2 = false, scene3 = true;
static int selectedConfig = -1;
static char configName[128] = "MyConfig";

static const char *renderOptions[] = {"OpenGL", "DirectX", "Vulkan", "Metal"};

void CompilerView::on_draw()
{
    std::string new_name = "Compiler##" + std::to_string(window_id);

    ImGui::Begin(new_name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

    // --- Título principal ---
    {
        // Puedes usar otra fuente si deseas
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "Game Compiler Interface");
        ImGui::PopFont();
    }

    ImGui::Separator();
    ImGui::Spacing();

    // --------------------------------------------------------------------
    // CREAR UNA TAB BAR PARA ORGANIZAR LAS SECCIONES
    // --------------------------------------------------------------------
    if (ImGui::BeginTabBar("##CompilerTabBar", ImGuiTabBarFlags_None))
    {
        // ======================= [1] GAME INFO TAB =======================
        if (ImGui::BeginTabItem("Game Info"))
        {
            ImGui::Spacing();
            ImGui::Text("Game Information");
            ImGui::Separator();
            ImGui::Spacing();

            // Grupo para "Name" e "Image Path"
            ImGui::BeginGroup();
            {
                ImGui::Text("Game Name:");
                ImGui::SameLine(120.0f);
                ImGui::SetNextItemWidth(250.0f);
                ImGui::InputTextWithHint("##gameName", "Enter the game name", gameName, IM_ARRAYSIZE(gameName));
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Specify the name of your game.");

                ImGui::Spacing();

                ImGui::Text("Image Path:");
                ImGui::SameLine(120.0f);
                ImGui::SetNextItemWidth(250.0f);
                ImGui::InputTextWithHint("##gameImagePath", "Enter the image path", gameImagePath, IM_ARRAYSIZE(gameImagePath));
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Provide the file path to the game's image.");

                ImGui::Spacing();

                // Botón para cargar imagen
                {
                    // Centrar el botón con algo de indentación
                    float indentSize = (ImGui::GetContentRegionAvail().x - 150.0f) * 0.5f;
                    ImGui::Indent(std::max(indentSize, 0.0f));
                    if (ImGui::Button("Load Image", ImVec2(150, 0)))
                    {
                        printf("Load Image button pressed. Image Path: %s\n", gameImagePath);
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("Click to load the specified image.");
                    ImGui::Unindent(std::max(indentSize, 0.0f));
                }
            }
            ImGui::EndGroup();

            ImGui::EndTabItem();
        }

        // ======================= [2] RENDER TAB ==========================
        if (ImGui::BeginTabItem("Render"))
        {
            ImGui::Spacing();
            ImGui::Text("Render Options");
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Text("Graphics Renderer:");
            ImGui::SetNextItemWidth(200.0f);
            ImGui::Combo("##renderOption", &renderOption, renderOptions, IM_ARRAYSIZE(renderOptions));
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Select the graphics rendering API to use.");

            ImGui::EndTabItem();
        }

        // ======================= [3] SCENES TAB ==========================
        if (ImGui::BeginTabItem("Scenes"))
        {
            ImGui::Spacing();
            ImGui::Text("Scenes to Compile");
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Text("Select Scenes to include in the build:");
            ImGui::Spacing();

            // Dibuja checkboxes en una tabla básica (opcional)
            if (ImGui::BeginTable("ScenesTable", 3, ImGuiTableFlags_SizingFixedFit))
            {
                ImGui::TableNextColumn();
                ImGui::Checkbox("Scene 1", &scene1);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Include Scene 1 in the compilation.");

                ImGui::TableNextColumn();
                ImGui::Checkbox("Scene 2", &scene2);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Include Scene 2 in the compilation.");

                ImGui::TableNextColumn();
                ImGui::Checkbox("Scene 3", &scene3);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Include Scene 3 in the compilation.");

                ImGui::EndTable();
            }

            ImGui::EndTabItem();
        }

        // ======================= [4] CONFIGS TAB =========================
        if (ImGui::BeginTabItem("Configurations"))
        {
            ImGui::Spacing();
            ImGui::Text("Configurations");
            ImGui::Separator();
            ImGui::Spacing();

            // Lista de configs
            ImGui::Text("Available Configurations:");
            std::vector<std::string> configs = listConfigs(configPath);

            if (configs.empty())
            {
                ImGui::TextDisabled("No available configurations found.");
            }
            else
            {
                // Podríamos usar BeginChild para que la lista sea scrollable
                ImGui::BeginChild("ConfigList", ImVec2(300, 100), true, ImGuiWindowFlags_HorizontalScrollbar);
                {
                    for (size_t i = 0; i < configs.size(); ++i)
                    {
                        if (ImGui::Selectable(configs[i].c_str(), selectedConfig == (int)i))
                        {
                            selectedConfig = (int)i;
                            strncpy(configName, configs[i].c_str(), IM_ARRAYSIZE(configName));
                        }
                    }
                }
                ImGui::EndChild();
            }

            ImGui::Spacing();
            ImGui::Text("Configuration Name:");
            ImGui::SetNextItemWidth(300.0f);
            ImGui::InputTextWithHint("##configName", "Enter configuration name", configName, IM_ARRAYSIZE(configName));
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Specify a name for the configuration.");

            ImGui::Spacing();

            // Botones: Save y Load
            {
                float buttonWidth = 150.0f;
                float totalWidth = ImGui::GetContentRegionAvail().x;
                float spacing = 10.0f;
                float groupWidth = (buttonWidth * 2.0f) + spacing;

                // Centrar los botones en la ventana
                float indentSize = (totalWidth - groupWidth) * 0.5f;
                ImGui::Indent(std::max(indentSize, 0.0f));

                if (ImGui::Button("Save Configuration", ImVec2(buttonWidth, 0)))
                {
                    std::string configNameStr(configName);
                    if (configNameStr.empty())
                    {
                        printf("Configuration name cannot be empty.\n");
                    }
                    else
                    {
                        if (configNameStr.find(".cconfig") == std::string::npos)
                            configNameStr += ".cconfig";

                        std::string filePath = configPath + configNameStr;

                        if (fs::exists(filePath))
                        {
                            printf("Configuration '%s' already exists. Overwriting.\n", filePath.c_str());
                        }

                        // Guardar la config
                        saveConfig(filePath.c_str(), gameName, gameImagePath, renderOption, scene1, scene2, scene3);
                        printf("Configuration saved as '%s'\n", filePath.c_str());

                        selectedConfig = (int)configs.size();
                    }
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Save the current settings as a configuration file.");

                ImGui::SameLine(0, spacing);

                if (ImGui::Button("Load Configuration", ImVec2(buttonWidth, 0)) && selectedConfig != -1)
                {
                    std::string selectedConfigPath = configPath + configs[selectedConfig];
                    loadConfig(selectedConfigPath.c_str(),
                               gameName, IM_ARRAYSIZE(gameName),
                               gameImagePath, IM_ARRAYSIZE(gameImagePath),
                               renderOption, scene1, scene2, scene3);

                    printf("Configuration loaded from '%s'\n", configs[selectedConfig].c_str());
                    strncpy(configName, configs[selectedConfig].c_str(), IM_ARRAYSIZE(configName));
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Load the selected configuration file.");

                ImGui::Unindent(std::max(indentSize, 0.0f));
            }

            ImGui::EndTabItem();
        }

        // ======================= [5] COMPILATION TAB =====================
        if (ImGui::BeginTabItem("Compilation"))
        {
            ImGui::Spacing();
            ImGui::Text("Compilation");
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::TextWrapped("Click 'Compile' to build the game with the selected options.\n"
                               "You can check the console for logs.");

            ImGui::Spacing();

            // Botón grande para compilar
            {
                float availWidth = ImGui::GetContentRegionAvail().x;
                float buttonWidth = 200.0f;
                ImGui::Indent(std::max((availWidth - buttonWidth) * 0.5f, 0.0f));

                if (ImGui::Button("Compile", ImVec2(buttonWidth, 40)))
                {
                    printf("Compiling the game '%s' using %s with the following scenes:\n",
                           gameName, renderOptions[renderOption]);
                    if (scene1)
                        printf("  - Scene 1\n");
                    if (scene2)
                        printf("  - Scene 2\n");
                    if (scene3)
                        printf("  - Scene 3\n");

                    // Ejemplo de proceso de compilación
                    std::thread compile_thread(WindowsCompiler::compile_windows);
                    compile_thread.join();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Click to start compiling the game with the selected settings.");

                ImGui::Unindent(std::max((availWidth - buttonWidth) * 0.5f, 0.0f));
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar(); // Fin de la TabBar
    }

    ImGui::End(); // Fin de la ventana principal
}
