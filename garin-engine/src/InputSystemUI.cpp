#include "../includes/InputSystemUI.h"

void InputSystemUI::draw()
{
    if (is_open)
    {
        ImGui::Begin("Input System");

        if (ImGui::Button("Add Axis"))
        {
        }

        ImGui::SameLine();

        if (ImGui::Button("Add Key"))
        {
        }

        ImGui::End();
    }
}