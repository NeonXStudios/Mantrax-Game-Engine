#include <CompilerView.h>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <imgui.h>
#include <stdio.h>
#include <WindowsCompiler.h>

namespace fs = std::filesystem;

void CompilerView::on_draw()
{
    ImGui::Begin("Compiler", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "Game Compiler Interface");
    ImGui::PopFont();
    ImGui::Separator();
    ImGui::Spacing();

    static char gameName[128] = "MyGame";
    static char gameImagePath[256] = "";

    if (ImGui::CollapsingHeader("ℹ️ Game Information", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Game Name:");
        ImGui::InputTextWithHint("##gameName", "Enter the game name", gameName, IM_ARRAYSIZE(gameName));
        ImGui::SameLine();
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Specify the name of your game.");

        ImGui::Spacing();

        ImGui::Text("Image Path:");
        ImGui::InputTextWithHint("##gameImagePath", "Enter the image path", gameImagePath, IM_ARRAYSIZE(gameImagePath));
        ImGui::SameLine();
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Provide the file path to the game's image.");

        ImGui::Spacing();

        if (ImGui::Button("Load Image", ImVec2(150, 0)))
        {
            printf("Load Image button pressed. Image Path: %s\n", gameImagePath);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Click to load the specified image.");
    }

    ImGui::Separator();
    ImGui::Spacing();

    static int renderOption = 0;
    const char *renderOptions[] = {"OpenGL", "DirectX", "Vulkan", "Metal"};
    if (ImGui::CollapsingHeader("Render Options", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Graphics Renderer:");
        ImGui::Combo("##renderOption", &renderOption, renderOptions, IM_ARRAYSIZE(renderOptions));
        ImGui::SameLine();
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Select the graphics rendering API to use.");
    }

    ImGui::Separator();
    ImGui::Spacing();

    static bool scene1 = true, scene2 = false, scene3 = true;
    if (ImGui::CollapsingHeader("Scenes to Compile", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Select Scenes:");
        ImGui::Checkbox("Scene 1", &scene1);
        ImGui::SameLine();
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Include Scene 1 in the compilation.");
        ImGui::Checkbox("Scene 2", &scene2);
        ImGui::SameLine();
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Include Scene 2 in the compilation.");
        ImGui::Checkbox("Scene 3", &scene3);
        ImGui::SameLine();
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Include Scene 3 in the compilation.");
    }

    ImGui::Separator();
    ImGui::Spacing();

    static int selectedConfig = -1;
    static char configName[128] = "MyConfig";

    if (ImGui::CollapsingHeader("Configurations", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Available Configurations:");
        std::vector<std::string> configs = listConfigs(configPath);

        if (configs.empty())
        {
            ImGui::TextDisabled("No available configurations found.");
        }
        else
        {
            for (size_t i = 0; i < configs.size(); ++i)
            {
                if (ImGui::Selectable(configs[i].c_str(), selectedConfig == i))
                {
                    selectedConfig = static_cast<int>(i);
                    strncpy(configName, configs[i].c_str(), IM_ARRAYSIZE(configName));
                }
            }
        }

        ImGui::Spacing();

        ImGui::Text("Configuration Name:");
        ImGui::InputTextWithHint("##configName", "Enter configuration name", configName, IM_ARRAYSIZE(configName));
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Specify a name for the configuration.");

        ImGui::Spacing();

        if (ImGui::Button("Save Configuration", ImVec2(150, 0)))
        {
            std::string configNameStr(configName);
            if (configNameStr.empty())
            {
                printf("Configuration name cannot be empty.\n");
            }
            else
            {
                if (configNameStr.find(".cconfig") == std::string::npos)
                {
                    configNameStr += ".cconfig";
                }

                std::string filePath = configPath + configNameStr;

                if (fs::exists(filePath))
                {
                    printf("Configuration '%s' already exists. Overwriting.\n", filePath.c_str());
                }

                saveConfig(filePath.c_str(), gameName, gameImagePath, renderOption, scene1, scene2, scene3);
                printf("Configuration saved as '%s'\n", filePath.c_str());

                selectedConfig = static_cast<int>(configs.size());
            }
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Save the current settings as a configuration file.");

        ImGui::SameLine();

        if (ImGui::Button("Load Configuration", ImVec2(150, 0)) && selectedConfig != -1)
        {
            std::string selectedConfigPath = configPath + configs[selectedConfig];
            loadConfig(selectedConfigPath.c_str(), gameName, IM_ARRAYSIZE(gameName), gameImagePath, IM_ARRAYSIZE(gameImagePath), renderOption, scene1, scene2, scene3);
            printf("Configuration loaded from '%s'\n", configs[selectedConfig].c_str());

            strncpy(configName, configs[selectedConfig].c_str(), IM_ARRAYSIZE(configName));
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Load the selected configuration file.");
    }

    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Compilation", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::Button("Compile", ImVec2(-1, 40)))
        {
            printf("Compiling the game '%s' using %s with the following scenes:\n", gameName, renderOptions[renderOption]);
            if (scene1)
                printf("  - Scene 1\n");
            if (scene2)
                printf("  - Scene 2\n");
            if (scene3)
                printf("  - Scene 3\n");

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

        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Click to start compiling the game with the selected settings.");
    }

    ImGui::End();
}
