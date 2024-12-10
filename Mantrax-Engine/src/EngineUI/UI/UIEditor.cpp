#include <UIEditor.h>
#include <Gfx.h>

#include <RenderPipeline.h>

void UIEditor::on_draw()
{
    ImGui::Begin("UI Editor", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);

    ImGuiID dockspace_id = ImGui::GetID("UIEditorDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

    ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
    ImGui::Begin("UI Elements", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    float calc_x = ImGui::GetWindowSize().x - 5;

    ImGui::Button("Image", ImVec2(calc_x, 50));

    if (ImGui::Button("Text", ImVec2(calc_x, 50)))
    {
        RenderPipeline::canvas->add_new_ui_component_type_text();
    }

    ImGui::Button("Button", ImVec2(calc_x, 50));
    ImGui::End();

    ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
    ImGui::Begin("SCENE", nullptr, ImGuiWindowFlags_NoCollapse);
    ImGui::Image((void *)(intptr_t)Gfx::main_render->get_render(),
                 ImVec2(Gfx::render_width, Gfx::render_height),
                 ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();

    ImGui::End();
}
