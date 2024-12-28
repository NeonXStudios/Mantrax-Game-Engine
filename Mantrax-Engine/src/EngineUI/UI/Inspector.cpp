#include <Inspector.h>
#include <GarinComponents.h>
#include <EditorGUI.h>
#include <EngineUI.h>
#include <UIAdministrator.h>

void Inspector::on_draw()
{
    if (EngineUI::getInstance().select_obj == nullptr)
        return;

    // Ventana principal
    ImGui::Begin("Inspector", &is_open, ImGuiWindowFlags_NoCollapse);

    // Sección del nombre del objeto
    {
        static char nameBuf[256];
        strcpy(nameBuf, EngineUI::getInstance().select_obj->ObjectName.c_str());
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::InputText("##Name", nameBuf, sizeof(nameBuf)))
        {
            EngineUI::getInstance().select_obj->ObjectName = nameBuf;
        }

        ImGui::Spacing();
    }

    // Tag y Layer
    {
        // Tag
        ImGui::TextDisabled("Tag");
        static char tagBuf[128];
        strcpy(tagBuf, EngineUI::getInstance().select_obj->ObjectTag.c_str());
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.45f);
        if (ImGui::InputText("##Tag", tagBuf, sizeof(tagBuf)))
        {
            EngineUI::getInstance().select_obj->ObjectTag = tagBuf;
        }

        ImGui::SameLine();
        float spacing = ImGui::GetContentRegionAvail().x * 0.1f;
        ImGui::Dummy(ImVec2(spacing, 0));
        ImGui::SameLine();

        // Layer
        ImGui::TextDisabled("Layer");
        int currentLayer = 0;
        for (int i = 0; i < layerNames.size(); ++i)
        {
            if (EngineUI::getInstance().select_obj->Layer == (1 << i))
            {
                currentLayer = i;
                break;
            }
        }
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Combo("##Layer", &currentLayer, layerNames.data(), static_cast<int>(layerNames.size())))
        {
            EngineUI::getInstance().select_obj->Layer = (1 << currentLayer);
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Transform
    {
        ImGui::TextDisabled("TRANSFORM");
        ImGui::Spacing();

        TransformComponent *transform = EngineUI::getInstance().select_obj->get_transform();

        // Position
        ImGui::TextDisabled("Position");
        transform->Position = EditorGUI::Vector3("##Position", transform->Position);
        ImGui::Spacing();

        // Rotation
        ImGui::TextDisabled("Rotation");
        glm::vec3 eulerAngles = transform->EulerRotation;
        eulerAngles = EditorGUI::Vector3("##Rotation", eulerAngles);
        transform->set_rotation(eulerAngles);
        ImGui::Spacing();

        // Scale
        ImGui::TextDisabled("Scale");
        transform->Scale = EditorGUI::Vector3("##Scale", transform->Scale);

        transform->update();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Components
    UIAdministrator::draw_ui(EngineUI::getInstance().select_obj);

    // Add Component Button
    {
        ImGui::Spacing();

        // Asegurarse de que el botón tenga un ancho válido
        float buttonWidth = ImGui::GetContentRegionAvail().x;
        if (buttonWidth <= 0)
            buttonWidth = 100; // Valor mínimo por defecto

        if (ImGui::Button("Add Component", ImVec2(buttonWidth, 30)))
        {
            ImGui::OpenPopup("AddComponentPopup");
        }

        if (ImGui::BeginPopup("AddComponentPopup"))
        {
            ImGui::TextDisabled("ADD COMPONENT");
            ImGui::Separator();
            ImGui::Spacing();

            // Asegurar un ancho mínimo para el popup
            float popupWidth = ImGui::GetContentRegionAvail().x;
            if (popupWidth < 150)
                popupWidth = 150;

            // Componentes disponibles
            if (ImGui::Button("Render Mesh", ImVec2(popupWidth, 0)))
            {
                EngineUI::getInstance().select_obj->addComponent<ModelComponent>().init();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Rigid Body", ImVec2(popupWidth, 0)))
            {
                EngineUI::getInstance().select_obj->addComponent<GBody>().init();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Box Collider", ImVec2(popupWidth, 0)))
            {
                EngineUI::getInstance().select_obj->addComponent<GCollision>().init();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Material", ImVec2(popupWidth, 0)))
            {
                EngineUI::getInstance().select_obj->addComponent<GMaterial>().init();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Game Script", ImVec2(popupWidth, 0)))
            {
                EngineUI::getInstance().select_obj->addComponent<GScript>().init();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Character Controller", ImVec2(popupWidth, 0)))
            {
                EngineUI::getInstance().select_obj->addComponent<GCharacter>().init();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Audio Source", ImVec2(popupWidth, 0)))
            {
                EngineUI::getInstance().select_obj->addComponent<GAudio>().init();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Perlin Component", ImVec2(popupWidth, 0)))
            {
                EngineUI::getInstance().select_obj->addComponent<GNoise>().init();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Animator", ImVec2(popupWidth, 0)))
            {
                EngineUI::getInstance().select_obj->addComponent<GAnimator>().init();
                ImGui::CloseCurrentPopup();
            }
            if (!EngineUI::getInstance().select_obj->hasComponent<GCamera>())
            {
                if (ImGui::Button("Camera", ImVec2(popupWidth, 0)))
                {
                    EngineUI::getInstance().select_obj->addComponent<GCamera>().init();
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }
    }

    ImGui::End();
}