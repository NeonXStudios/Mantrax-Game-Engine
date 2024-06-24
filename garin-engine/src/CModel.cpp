#include "../includes/CModel.h"
#include <GarinComponents.h>

namespace fs = std::filesystem;

void CModel::start()
{
}

void CModel::draw(Entity *owner)
{
    auto modelComponents = owner->getComponents<ModelComponent>();
    for (auto *cmap : modelComponents)
    {
        ImGui::PushID(cmap->component_id);
        std::string componentName = "Render Mesh";

        bool enabledCTMP = cmap->enabled;
        ImGui::Checkbox("", &enabledCTMP);
        cmap->enabled = enabledCTMP;

        ImGui::SameLine();

        bool treeNodeOpen = ImGui::TreeNodeEx(componentName.c_str(), ImGuiTreeNodeFlags_SpanAvailWidth);

        ImGui::SameLine(ImGui::GetContentRegionMax().x - 20);
        if (ImGui::Button("X"))
        {
            std::cout << "Trying deleting component" << std::endl;
        }

        if (treeNodeOpen)
        {
            ImGui::Separator();
            ImGui::Text("Model");

            std::string filePath = cmap->model_path;

            size_t pos = filePath.find("assets");
            if (pos != std::string::npos)
            {
                filePath = filePath.substr(pos);
            }

            std::string fileName = filePath.substr(filePath.find_last_of("/\\") + 1);

            char fileNameBuffer[256];
            strncpy(fileNameBuffer, fileName.c_str(), sizeof(fileNameBuffer));
            fileNameBuffer[sizeof(fileNameBuffer) - 1] = '\0';

            float inputTextWidth = ImGui::GetContentRegionAvail().x - 60.0f;

            ImGui::BeginGroup();

            ImGui::Image((void *)(intptr_t)1, ImVec2(16, 16));
            ImGui::SameLine();

            ImGui::PushItemWidth(inputTextWidth);
            ImGui::InputText("##FileName", fileNameBuffer, sizeof(fileNameBuffer), ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();

            ImGui::SameLine();
            if (ImGui::Button("..."))
            {
                ImGui::OpenPopup("File Browser");
            }

            ImGui::EndGroup();

            auto selectedFilePath = FileBrowser::DrawFileBrowser();
            if (selectedFilePath.has_value())
            {
                std::string selectedPath = selectedFilePath.value();
                size_t pos = selectedPath.find("assets");
                if (pos != std::string::npos)
                {
                    selectedPath = selectedPath.substr(pos);
                }

                std::cout << "SELECT FILE PATH: " << selectedPath << std::endl;
                cmap->model_path = selectedPath;
            }

            ImGui::Separator();
            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}
