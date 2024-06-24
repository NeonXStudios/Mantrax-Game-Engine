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

        ImGui::DragFloat3(EditorGUI::_labelPrefix(Name.c_str()).c_str(), v);

        return glm::quat(glm::vec3(glm::radians(v[0]), glm::radians(v[1]), glm::radians(v[2])));
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