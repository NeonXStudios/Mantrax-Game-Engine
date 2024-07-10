#include "../includes/InspectorUI.h"
#include "../includes/UIAdministrator.h"
#include <GarinComponents.h>

#include <GarinNatives.h>
#include <GarinUI.h>

void InspectorUI::draw(Entity *select_obj)
{
    if (select_obj != nullptr)
    {
        ImGui::Begin("Inspector");
        select_obj->ObjectName = EditorGUI::InputText("Name: ", select_obj->ObjectName);
        select_obj->get_transform()->Position = EditorGUI::Vector3("Position", select_obj->get_transform()->Position);
        select_obj->get_transform()->update();
        // select_obj->get_transform()->Rotation = EditorGUI::Vector3("Rotation", select_obj->get_transform()->Rotation);
        select_obj->get_transform()->Scale = EditorGUI::Vector3("Scale", select_obj->get_transform()->Scale);
        select_obj->get_transform()->update();

        ImGui::Separator();

        UIAdministrator::draw_ui(select_obj);

        ImGui::Separator();

        float buttonWidth = ImGui::GetContentRegionAvail().x;

        if (ImGui::Button("Add Component", ImVec2(buttonWidth, 20)))
        {
            ImGui::OpenPopup("AddComponentPopup");
        }

        if (ImGui::BeginPopup("AddComponentPopup"))
        {
            ImGui::Text("Select a component to add:");
            float buttonWidth = ImGui::GetContentRegionAvail().x;

            if (ImGui::Button("Render Mesh", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<ModelComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Rigid Body", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<GBody>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Box Collider", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<GCollision>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Material", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<GMaterial>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Game Script", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<GScript>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Character Controller", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<GCharacter>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Audio Source", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<GAudio>();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        ImGui::End();
    }
}