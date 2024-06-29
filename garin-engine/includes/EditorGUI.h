#pragma once

#include <iostream>
#include <GarinNatives.h>
#include <GarinUI.h>

using namespace std;

class EditorGUI
{
public:
    static string InputText(string Name, string value)
    {
        char GetName[128];
        strcpy_s(GetName, value.c_str());
        ImGui::InputText(EditorGUI::_labelPrefix(Name.c_str()).c_str(), GetName, 50000, ImGuiInputTextFlags_AutoSelectAll);

        return ((std::string)GetName);
    }

    static string Text(string text)
    {
        ImGui::Text(text.c_str());
        return text;
    }

    static float Float(const std::string &name, float value)
    {
        ImGui::DragFloat(EditorGUI::_labelPrefix(name.c_str()).c_str(), &value);
        return value;
    }

    static float Int(const std::string &name, int value)
    {
        ImGui::DragInt(EditorGUI::_labelPrefix(name.c_str()).c_str(), &value);
        return value;
    }

    static glm::vec2 Vector2(string Name, glm::vec2 vector)
    {
        float v[2] = {
            vector.x,
            vector.y};

        ImGui::DragFloat2(EditorGUI::_labelPrefix(Name.c_str()).c_str(), v);

        return glm::vec2(v[0], v[1]);
    }

    static bool Toggle(string Name, bool valueB)
    {
        ImGui::Checkbox(Name.c_str(), &valueB);
        return valueB;
    }

    static glm::vec3 Vector3(string Name, glm::vec3 vector)
    {
        float v[3] = {
            vector.x,
            vector.y,
            vector.z};

        ImGui::DragFloat3(EditorGUI::_labelPrefix(Name.c_str()).c_str(), v);

        return glm::vec3(v[0], v[1], v[2]);
    }

    static glm::vec4 Vector4(string Name, glm::vec4 vector)
    {
        float v[4] = {
            vector.x,
            vector.y,
            vector.z,
            vector.w};

        ImGui::DragFloat4(EditorGUI::_labelPrefix(Name.c_str()).c_str(), v);

        return glm::vec4(v[0], v[1], v[2], v[3]);
    }

    static glm::quat Quaternion(string Name, glm::quat quaternion)
    {
        float v[3] = {
            quaternion.x,
            quaternion.y,
            quaternion.z};

        ImGui::DragFloat3(EditorGUI::_labelPrefix(Name.c_str()).c_str(), v, 0.0001f);

        return glm::quat(glm::vec3((v[0]), (v[1]), (v[2])));
    }

    static bool Button(string Name, glm::vec2 size)
    {
        return ImGui::Button(Name.c_str(), ImVec2(size.x, size.y));
    }

    static float Slider(string name, float value, float min, float max)
    {
        ImGui::SliderFloat(_labelPrefix(name.c_str()).c_str(), &value, min, max);
        return value;
    }

    template <typename T>
    static void Draw_Component(Entity *owner, T *cpm, string componentName, int ID, Component *component, std::function<void(void)> func)
    {
        ImGui::PushID(ID);

        bool enabledCTMP = component->enabled;
        ImGui::Checkbox("", &enabledCTMP);
        component->enabled = enabledCTMP;

        ImGui::SameLine();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 2.0f));

        bool treeNodeOpen = ImGui::TreeNodeEx(componentName.c_str());

        ImGui::PopStyleVar(2);

        ImGui::SameLine(ImGui::GetContentRegionMax().x - 20);
        if (ImGui::Button("X"))
        {
            owner->removeComponent<T>();
        }
        else
        {
            if (treeNodeOpen)
            {
                ImGui::Separator();
                // ImGui::Text(componentName.c_str());
                float inputTextWidth = ImGui::GetContentRegionAvail().x - 60.0f;

                ImGui::BeginGroup();

                ImGui::SameLine();
                func();
                ImGui::EndGroup();
                ImGui::Separator();
                ImGui::TreePop();
            }
        }

        ImGui::PopID();
    }

    template <typename T, typename MemberType>
    static void Draw_Input_Field_Model(const std::string &Name, T *component, MemberType T::*member)
    {
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Text(Name.c_str());

        std::string &filePath = component->*member;

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
            component->*member = selectedPath;
        }
    }

    static std::string _labelPrefix(const char *const label)
    {
        float width = ImGui::CalcItemWidth();

        float x = ImGui::GetCursorPosX();
        ImGui::Text(label);
        ImGui::SameLine();
        ImGui::SetCursorPosX(x + width * 0.5f + ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::SetNextItemWidth(-1);

        std::string labelID = "##";
        labelID += label;

        return labelID;
    }
};