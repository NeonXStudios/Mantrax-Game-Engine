#include "../includes/SceneUI.h"
#include <GarinGraphics.h>
#include <GarinEvents.h>
#include <GarinMaths.h>
#include <EventSystem.h>
#include <RenderPipeline.h>

using namespace ImGuizmo;

ImGuizmo::MODE gizmoMode(ImGuizmo::LOCAL);
ImGuizmo::OPERATION gizmoOperation(ImGuizmo::TRANSLATE);

void SceneUI::draw(Entity *select_obj)
{
    ImGui::PushID(343456);

    ImGui::Begin("Scene", &is_open);

    ImGuizmo::SetDrawlist();

    ImVec2 windowSize = ImGui::GetContentRegionAvail();

    ImVec2 p = ImGui::GetCursorScreenPos();
    ImGui::Image((void *)(intptr_t)Gfx::main_render->get_render(), ImVec2(Gfx::render_width, Gfx::render_height), ImVec2(ImVec2(0, 1)), ImVec2(ImVec2(1, 0)));
    imagePosition = ImGui::GetWindowPos();
    Gfx::render_width = windowSize.x;
    Gfx::render_height = windowSize.y;

    try
    {
        if (ImGui::IsWindowHovered() && game != nullptr)
        {
            CastData *data = new CastData();

            if (ImGui::IsMouseClicked(0))
            {
                WorldPoint = EventSystem::ScreenToViewPort(glm::vec2(p.x, p.y), glm::vec2(Gfx::render_width, Gfx::render_height));

                if (EventSystem::MouseCast(WorldPoint, data))
                {
                    std::cout << "Selecting object: " << data->object->ObjectName << std::endl;

                    if (select_obj != data->object && game != nullptr)
                    {
                        game->set_object_select(data->object);
                    }
                }
                else
                {
                    if (!ImGuizmo::IsOver())
                    {
                        if (game != nullptr)
                        {
                            std::cout << "Removing object from select" << std::endl;
                            game->set_object_select(nullptr);
                        }
                    }
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    if (select_obj != nullptr)
    {
        static ImGuizmo::MODE gizmoMode(ImGuizmo::LOCAL);
        static ImGuizmo::OPERATION gizmoOperation(ImGuizmo::TRANSLATE);

        TransformComponent *transform = select_obj->get_transform();

        glm::mat4 matrix = transform->get_matrix();

        float *projection = (float *)glm::value_ptr(SceneManager::GetOpenScene()->main_camera->GetProjectionMatrix());
        float *view = (float *)glm::value_ptr(SceneManager::GetOpenScene()->main_camera->GetView());

        ImGuizmo::SetRect(p.x, p.y, Gfx::render_width, Gfx::render_height);

        bool res = ImGuizmo::Manipulate(view, projection, gizmoOperation, gizmoMode, glm::value_ptr(matrix));
        ignoreGui &= !ImGuizmo::IsOver();

        if (res)
        {
            glm::vec3 translation, eulerRotation, scale;
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(matrix), glm::value_ptr(translation), glm::value_ptr(eulerRotation), glm::value_ptr(scale));

            if (!select_obj->hasComponent<GBody>())
            {
                transform->Position = translation;
                transform->Scale = scale;

                // Establece la rotación utilizando ángulos de Euler en lugar de cuaterniones
                transform->set_rotation(eulerRotation);
            }
            else
            {
                transform->Position = translation;
                transform->Scale = scale;

                // Establece la rotación utilizando ángulos de Euler en lugar de cuaterniones
                transform->set_rotation(eulerRotation);

                // Actualizar componentes físicos
                select_obj->getComponent<GBody>().body->setLinearVelocity(PxVec3(0, 0, 0));
                select_obj->getComponent<GBody>().set_position(translation);
            }
        }

        if (!ImGui::IsMouseDown(1))
        {
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
    }

    if (ImGui::IsKeyDown(ImGuiKey_Delete))
    {
        SceneManager::GetOpenScene()->destroy(select_obj);
        select_obj = nullptr;
    }

    if (ImGui::IsWindowHovered() && ImGui::IsKeyDown(ImGuiKey_F) && select_obj != nullptr)
    {
        SceneManager::GetOpenScene()->main_camera->cameraPosition = select_obj->get_transform()->Position;
    }

    ImGui::End();
    ImGui::PopID();

    ImGui::PushID(554345);
    ImGui::Begin("Game", &is_open);

    ImGuizmo::SetDrawlist();

    Camera *camera_with_max_depth = nullptr;
    float max_depth = -1.0f;

    for (Entity *ent : SceneManager::GetOpenScene()->objects_worlds)
    {
        if (ent->hasComponent<GCamera>())
        {
            GCamera &camera_component = ent->getComponent<GCamera>();
            Camera *camera = camera_component.a_camera;

            int current_depth = camera_component.Depth;

            if (current_depth > max_depth)
            {
                max_depth = current_depth;
                camera_with_max_depth = camera;
            }
        }
    }

    if (camera_with_max_depth != nullptr)
    {
        ImVec2 windowSize = ImGui::GetContentRegionAvail();

        TextureTarget *max_depth_render_target = camera_with_max_depth->target_render;
        ImGui::Image((void *)(intptr_t)max_depth_render_target->get_render(), ImVec2(Gfx::render_width, Gfx::render_height), ImVec2(0, 1), ImVec2(1, 0));
    }

    ImGui::End();
    ImGui::PopID();
}
