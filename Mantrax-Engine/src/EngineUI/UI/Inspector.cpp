#include <Inspector.h>
#include <GarinComponents.h>
#include <EditorGUI.h>
#include <EngineUI.h>
#include <UIAdministrator.h>

void Inspector::on_draw()
{
    if (EngineUI::getInstance().select_obj != nullptr)
    {
        ImGui::Begin("Inspector", &is_open);

        int currentLayer = 0;
        for (int i = 0; i < layerNames.size(); ++i)
        {
            if (EngineUI::getInstance().select_obj->Layer == (1 << i))
            {
                currentLayer = i;
                break;
            }
        }

        EngineUI::getInstance().select_obj->ObjectTag = EditorGUI::InputText("Tag", EngineUI::getInstance().select_obj->ObjectTag);

        if (ImGui::Combo("Layer", &currentLayer, layerNames.data(), layerNames.size()))
        {
            EngineUI::getInstance().select_obj->Layer = 1 << currentLayer;
        }

        ImGui::Separator();

        EngineUI::getInstance().select_obj->ObjectName = EditorGUI::InputText("Name: ", EngineUI::getInstance().select_obj->ObjectName);
        EngineUI::getInstance().select_obj->get_transform()->Position = EditorGUI::Vector3("Position", EngineUI::getInstance().select_obj->get_transform()->Position);
        EngineUI::getInstance().select_obj->get_transform()->update();

        glm::vec3 eulerAngles = EngineUI::getInstance().select_obj->get_transform()->EulerRotation;
        eulerAngles = EditorGUI::Vector3("Rotation", eulerAngles);
        EngineUI::getInstance().select_obj->get_transform()->set_rotation(eulerAngles);
        EngineUI::getInstance().select_obj->get_transform()->update();

        EngineUI::getInstance().select_obj->get_transform()->Scale = EditorGUI::Vector3("Scale", EngineUI::getInstance().select_obj->get_transform()->Scale);
        EngineUI::getInstance().select_obj->get_transform()->update();

        ImGui::Separator();

        UIAdministrator::draw_ui(EngineUI::getInstance().select_obj);

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
                EngineUI::getInstance().select_obj->addComponent<ModelComponent>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Rigid Body", ImVec2(buttonWidth, 20)))
            {
                EngineUI::getInstance().select_obj->addComponent<GBody>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Box Collider", ImVec2(buttonWidth, 20)))
            {
                EngineUI::getInstance().select_obj->addComponent<GCollision>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Material", ImVec2(buttonWidth, 20)))
            {
                EngineUI::getInstance().select_obj->addComponent<GMaterial>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Game Script", ImVec2(buttonWidth, 20)))
            {
                EngineUI::getInstance().select_obj->addComponent<GScript>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Character Controller", ImVec2(buttonWidth, 20)))
            {
                EngineUI::getInstance().select_obj->addComponent<GCharacter>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Audio Source", ImVec2(buttonWidth, 20)))
            {
                EngineUI::getInstance().select_obj->addComponent<GAudio>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Perlin Component", ImVec2(buttonWidth, 20)))
            {
                EngineUI::getInstance().select_obj->addComponent<GNoise>().init();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Animator", ImVec2(buttonWidth, 20)))
            {
                EngineUI::getInstance().select_obj->addComponent<GAnimator>().init();
                ImGui::CloseCurrentPopup();
            }

            if (EngineUI::getInstance().select_obj->hasComponent<GCamera>() == false)
            {
                if (ImGui::Button("Camera", ImVec2(buttonWidth, 20)))
                {
                    EngineUI::getInstance().select_obj->addComponent<GCamera>().init();
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }
        ImGui::End();
    }
}