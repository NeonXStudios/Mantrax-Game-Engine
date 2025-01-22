#include <Inspector.h>
#include <GarinComponents.h>
#include <EditorGUI.h>
#include <EngineUI.h>
#include <UIAdministrator.h>

void Inspector::on_draw()
{
    if (EngineUI::getInstance().select_obj == nullptr)
        return;

    if (SceneManager::get_parent_scene_from_object(EngineUI::getInstance().select_obj) != SceneManager::get_current_scene()) 
        return;

    std::string new_name = "Inspector##" + std::to_string(window_id);
    ImGui::Begin(new_name.c_str(), &is_open, ImGuiWindowFlags_NoCollapse);

    // Nombre del objeto y botón +
    {
        ImGui::BeginGroup();
        static char nameBuf[256];
        strcpy(nameBuf, EngineUI::getInstance().select_obj->name_object.c_str());
        
        float availableWidth = ImGui::GetContentRegionAvail().x - 25;
        ImGui::SetNextItemWidth(availableWidth);
        
        if (ImGui::InputText("##Name", nameBuf, sizeof(nameBuf)))
        {
            EngineUI::getInstance().select_obj->name_object = nameBuf;
        }

        ImGui::SameLine(availableWidth + 5);
        
        // Centramos el texto "+" en el botón
        float buttonSize = 20;
        ImVec2 cursorPos = ImGui::GetCursorPos();
        if (ImGui::Button("##AddComponent", ImVec2(buttonSize, buttonSize)))
        {
            ImGui::OpenPopup("AddComponentPopup");
        }
        
        // Calculamos la posición para centrar el "+"
        ImVec2 textSize = ImGui::CalcTextSize("+");
        float textPosX = cursorPos.x + (buttonSize - textSize.x) * 0.5f;
        float textPosY = cursorPos.y + (buttonSize - textSize.y) * 0.5f;
        ImGui::GetWindowDrawList()->AddText(
            ImVec2(ImGui::GetWindowPos().x + textPosX, ImGui::GetWindowPos().y + textPosY),
            ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_Text]),
            "+"
        );
        
        ImGui::EndGroup();
        ImGui::Spacing();
    }

    // Tag y Layer
    {
        // Tag
        ImGui::TextDisabled("Tag");
        static char tagBuf[128];
        strcpy(tagBuf, EngineUI::getInstance().select_obj->object_tag.c_str());
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.45f);
        if (ImGui::InputText("##Tag", tagBuf, sizeof(tagBuf)))
        {
            EngineUI::getInstance().select_obj->object_tag = tagBuf;
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

        TransformComponent* transform = EngineUI::getInstance().select_obj->get_transform();

        // Position
        ImGui::TextDisabled("Position");
        transform->Position = EditorGUI::Vector3("##Position", transform->Position);
        ImGui::Spacing();

        // Rotation
        ImGui::TextDisabled("Rotation");
        glm::vec3 eulerAngles = transform->euler_rotation;
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

     ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopup("AddComponentPopup", ImGuiWindowFlags_NoMove))
    {
        ImGui::Text("Components");
        ImGui::Separator();
        ImGui::Spacing();

        // Lista de componentes con scroll
        ImGui::BeginChild("ComponentsList", ImVec2(600, 400), true);

        if (ImGui::Button("Render Mesh", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<ModelComponent>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Rigid Body", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GBody>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Box Collider", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GCollision>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Material", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GMaterial>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Game Script", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GScript>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Character Controller", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GCharacter>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Audio Source", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GAudio>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Perlin Component", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GNoise>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Animator", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GAnimator>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Point Light", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GLightPoint>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Spot Light", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GLightSpot>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Direction Light", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GLightDirectional>().init();
            ImGui::CloseCurrentPopup();
        }
        if (!EngineUI::getInstance().select_obj->hasComponent<GCamera>())
        {
            if (ImGui::Button("Camera", ImVec2(-1, 0)))
            {
                EngineUI::getInstance().select_obj->addComponent<GCamera>().init();
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndChild();
        ImGui::EndPopup();
    }

    ImGui::End();
}