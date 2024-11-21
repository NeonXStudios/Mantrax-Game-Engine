#include "../includes/InspectorUI.h"
#include "../includes/UIAdministrator.h"
#include <GarinComponents.h>

#include <RenderPipeline.h>
#include <GarinNatives.h>
#include <GarinUI.h>

void InspectorUI::draw(Entity *select_obj)
{
    if (select_obj != nullptr)
    {
        ImGui::Begin("Inspector", &is_open);

        int currentLayer = 0;
        for (int i = 0; i < layerNames.size(); ++i)
        {
            if (select_obj->Layer == (1 << i))
            {
                currentLayer = i;
                break;
            }
        }

        select_obj->ObjectTag = EditorGUI::InputText("Tag", select_obj->ObjectTag);

        if (ImGui::Combo("Layer", &currentLayer, layerNames.data(), layerNames.size()))
        {
            select_obj->Layer = 1 << currentLayer;
        }

        ImGui::Separator();

        select_obj->ObjectName = EditorGUI::InputText("Name: ", select_obj->ObjectName);
        select_obj->get_transform()->Position = EditorGUI::Vector3("Position", select_obj->get_transform()->Position);
        select_obj->get_transform()->update();

        glm::vec3 eulerAngles = select_obj->get_transform()->EulerRotation;
        eulerAngles = EditorGUI::Vector3("Rotation", eulerAngles);
        select_obj->get_transform()->set_rotation(eulerAngles);
        select_obj->get_transform()->update();

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
                select_obj->addComponent<ModelComponent>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Rigid Body", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<GBody>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Box Collider", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<GCollision>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Material", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<GMaterial>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Game Script (C++ Alpha)", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<GScript>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Game Script (Lua)", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<GScriptLua>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Character Controller", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<GCharacter>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Audio Source", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<GAudio>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Perlin Component", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<GNoise>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Animator", ImVec2(buttonWidth, 20)))
            {
                select_obj->addComponent<GAnimator>().init();
                ImGui::CloseCurrentPopup();
            }

            if (select_obj->hasComponent<GCamera>() == false)
            {
                if (ImGui::Button("Camera", ImVec2(buttonWidth, 20)))
                {
                    select_obj->addComponent<GCamera>().init();
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }
        ImGui::End();
    }
}