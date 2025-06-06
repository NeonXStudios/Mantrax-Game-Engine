#include <UIEditor.h>
#include <Gfx.h>
#include <RenderPipeline.h>
#include <ServiceLocator.h>

void UIEditor::on_draw()
{
    RenderPipeline *render_pipeline = ServiceLocator::get<RenderPipeline>().get();

    std::string new_name = "UIEditor##" + std::to_string(window_id);
    ImGui::Begin(new_name.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration);

    ImGuiID dockspace_id = ImGui::GetID("UIEditorDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

    ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
    ImGui::Begin("UI Elements", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::Button("Image", ImVec2(-1, 50));

    if (ImGui::Button("Text", ImVec2(-1, 50)))
    {
        render_pipeline->canvas->add_new_ui_component_type_text();
    }

    ImGui::Button("Button", ImVec2(-1, 50));
    ImGui::End();

    ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
    ImGui::Begin("SCENE", nullptr, ImGuiWindowFlags_NoCollapse);
    ImGui::Image((void *)(intptr_t)Gfx::main_render->get_render(),
                 ImVec2(Gfx::render_width, Gfx::render_height),
                 ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();

    ImGui::End();
}
