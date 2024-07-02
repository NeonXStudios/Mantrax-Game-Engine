#include "../includes/SceneUI.h"
#include <GarinGraphics.h>
#include <GarinEvents.h>

using namespace ImGuizmo;

ImGuizmo::MODE gizmoMode(ImGuizmo::LOCAL);
ImGuizmo::OPERATION gizmoOperation(ImGuizmo::TRANSLATE);

void SceneUI::draw(Entity *select_obj)
{
    ImGui::PushID(343456);

    ImGui::Begin("Scene");

    ImGuizmo::SetDrawlist();

    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    Graphics::render_width = windowSize.x;
    Graphics::render_height = windowSize.y;
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImGui::Image((void *)(intptr_t)Graphics::texture, ImVec2(Graphics::render_width, Graphics::render_height), ImVec2(ImVec2(0, 1)), ImVec2(ImVec2(1, 0)));

    if (select_obj != nullptr && !AppSettings::is_playing)
    {
        static ImGuizmo::MODE gizmoMode(ImGuizmo::LOCAL);
        static ImGuizmo::OPERATION gizmoOperation(ImGuizmo::TRANSLATE);

        TransformComponent *transform = select_obj->get_transform();

        glm::mat4 matrix = transform->get_matrix();

        float *projection = (float *)glm::value_ptr(Graphics::get_main_camera()->GetProjectionMatrix());
        float *view = (float *)glm::value_ptr(Graphics::get_main_camera()->GetView());

        ImGuizmo::SetRect(p.x, p.y, Graphics::render_width, Graphics::render_height);

        bool res = ImGuizmo::Manipulate(view, projection, gizmoOperation, gizmoMode, glm::value_ptr(matrix));
        ignoreGui &= !ImGuizmo::IsOver();

        if (res)
        {
            glm::vec3 translation, rotation, scale;
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(matrix), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));

            transform->Position = translation;
            transform->rotation.x = glm::radians(rotation.x);
            transform->rotation.y = glm::radians(rotation.y);
            transform->rotation.z = glm::radians(rotation.z);
            transform->Scale = scale;
            transform->update();
        }

        if (InputSystem::on_key_pressed(GLFW_KEY_W))
        {
            gizmoOperation = ImGuizmo::TRANSLATE;
        }

        if (InputSystem::on_key_pressed(GLFW_KEY_Q))
        {
            gizmoOperation = ImGuizmo::ROTATE;
        }

        if (InputSystem::on_key_pressed(GLFW_KEY_E))
        {
            gizmoOperation = ImGuizmo::SCALE;
        }
    }

    ImGui::End();
    ImGui::PopID();
}