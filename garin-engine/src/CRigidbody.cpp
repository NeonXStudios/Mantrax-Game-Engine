#include "../includes/CRigidbody.h"
#include <GarinComponents.h>

namespace fs = std::filesystem;

void CRigidbody::start()
{
}

void CRigidbody::draw(Entity *owner)
{
    auto modelComponents = owner->getComponents<GBody>();
    for (auto *cmap : modelComponents)
    {
        ImGui::PushID(cmap->component_id);
        std::string componentName = "Rigid body";

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
            ImGui::Text("Is Static");

            float inputTextWidth = ImGui::GetContentRegionAvail().x - 60.0f;

            ImGui::BeginGroup();

            // ImGui::Image((void *)(intptr_t)1, ImVec2(16, 16));
            ImGui::SameLine();

            ImGui::PushItemWidth(inputTextWidth);
            bool its = cmap->isStatic;
            ImGui::PushID("a1");
            ImGui::Checkbox("", &its);
            ImGui::PopID();
            cmap->isStatic = its;
            ImGui::PopItemWidth();

            ImGui::EndGroup();
            ImGui::Separator();
            ImGui::Text("Use Gravity");

            ImGui::BeginGroup();

            // ImGui::Image((void *)(intptr_t)1, ImVec2(16, 16));
            ImGui::SameLine();

            ImGui::PushItemWidth(inputTextWidth);
            bool grv = cmap->useGravity;
            ImGui::PushID("a2");
            ImGui::Checkbox("", &grv);
            ImGui::PopID();
            cmap->useGravity = grv;
            ImGui::PopItemWidth();

            ImGui::EndGroup();

            ImGui::Separator();

            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}
