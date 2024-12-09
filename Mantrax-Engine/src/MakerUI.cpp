#include "../includes/MakerUI.h"
#include <EngineUI.h>

void MakerUI::draw()
{
    if (!is_open)
    {
        return;
    }

    ImGui::Begin("UI Editor", &is_open);
    ImGui::Begin("UI Elements");
    float width = 200;
    float height = 25;

    if (ImGui::Button("Add Button", ImVec2(width, height)))
    {
    }
    if (ImGui::Button("Add Text", ImVec2(width, height)))
    {
    }
    if (ImGui::Button("Add Input", ImVec2(width, height)))
    {
    }
    if (ImGui::Button("Add Slider", ImVec2(width, height)))
    {
    }
    if (ImGui::Button("Add Toggle", ImVec2(width, height)))
    {
    }
    if (ImGui::Button("Add Scroll", ImVec2(width, height)))
    {
    }

    ImGui::End();

    ImVec2 windowSize = ImGui::GetContentRegionAvail();

    ImVec2 p = ImGui::GetCursorScreenPos();
    ImGui::Image((void *)(intptr_t)Gfx::main_render->get_render(), ImVec2(Gfx::render_width, Gfx::render_height), ImVec2(ImVec2(0, 1)), ImVec2(ImVec2(1, 0)));
    imagePosition = ImGui::GetWindowPos();

    ImGui::End();
}