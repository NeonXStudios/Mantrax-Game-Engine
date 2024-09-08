#include "../includes/gamescene.h"

#include <iostream>
#include <cstdarg>
#include <thread>
#include <SceneData.h>
#include <RenderPipeline.h>
#include <GarinIO.h>

void gamescene::on_start()
{
    old_stdout = std::cout.rdbuf(buffer_stdout.rdbuf());
    old_stderr = std::cerr.rdbuf(buffer_stderr.rdbuf());

    configs = new EditorConfigs();

    GarinUI::make_ui_context(Gfx::get_game_window());
    std::cout << "CARPETA DE EJECUCION: " << FileManager::get_execute_path() << std::endl;

    uieditor = new UIEditorManager();
    gizmos = new GizmosDrawer();

    uieditor->configs = configs;
    uieditor->game = this;

    if (uieditor->game != nullptr)
    {
        std::cout << "Correctly game scene setup in editor UI" << std::endl;
    }

    uieditor->setup();

    gizmos->config_line();

    IconsManager::init();

    string info = "OPEN FILE: " + configs->current_scene;

    UINotification::AddNotification(info, 3.0f);

    gizmo_models = new GizmoModels();
    gizmo_models->init();
}

void gamescene::on_edition_mode(float delta_time)
{
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && configs->project_select)
    {
        SceneManager::GetOpenScene()->main_camera->move_forward(delta_time, InputSystem::get_axis(GLFW_KEY_W, GLFW_KEY_S) * 10.0f * delta_time);
        SceneManager::GetOpenScene()->main_camera->move_left(delta_time, InputSystem::get_axis(GLFW_KEY_A, GLFW_KEY_D) * 10.0f * delta_time);

        if (InputSystem::on_key_pressed(GLFW_KEY_Q))
        {
            SceneManager::GetOpenScene()->main_camera->cameraPosition.y -= 10.0f * delta_time;
        }

        if (InputSystem::on_key_pressed(GLFW_KEY_E))
        {
            SceneManager::GetOpenScene()->main_camera->cameraPosition.y += 10.0f * delta_time;
        }

        glm::vec3 newOrientation = glm::rotate(SceneManager::GetOpenScene()->main_camera->Orientation, glm::radians(InputSystem::get_mouse_y() * camera_speed_sens * delta_time), glm::normalize(glm::cross(SceneManager::GetOpenScene()->main_camera->Orientation, SceneManager::GetOpenScene()->main_camera->GetUp())));

        if (abs(glm::angle(newOrientation, SceneManager::GetOpenScene()->main_camera->GetUp()) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            SceneManager::GetOpenScene()->main_camera->Orientation = newOrientation;
        }

        SceneManager::GetOpenScene()->main_camera->Orientation = glm::rotate(SceneManager::GetOpenScene()->main_camera->Orientation, glm::radians(-InputSystem::get_mouse_x() * camera_speed_sens * delta_time), SceneManager::GetOpenScene()->main_camera->GetUp());
    }

    std::string window_name = "Garin Editor - " + SceneManager::GetOpenScene()->scene_name;

    for (Entity *entity : objects_worlds)
    {
        entity->transform_component->update();
        for (Entity *transform : entity->childrens)
        {
            transform->transform_component->update();
        }
    }

    Gfx::change_name(window_name);
}

void gamescene::on_update(float delta_time)
{
    // GCastHit *hit = new GCastHit();

    // if (GCaster::LineCast(SceneManager::GetOpenScene()->get_entity_by_index(1)->get_transform()->Position, glm::vec3(0.0f, -1.0f, 0.0f), 100, hit, LAYER_1))
    // {
    //     std::cout << "Casting: " << hit->entity->ObjectName << std::endl;
    // }
}

void gamescene::on_draw()
{
    gizmos->draw_line(glm::vec3(0.0f), glm::vec3(20.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(5.0f));
    gizmo_models->draw();
}

void gamescene::draw_ui()
{
    GarinUI::get_ui_manager()->render_new_frame_ui_context(true);

    uieditor->draw();

    if (configs->project_select == true)
    {
        ReadBuffer(buffer_stdout, stdout_buffer);
        ReadBuffer(buffer_stderr, stderr_buffer);

        // notify->RenderNotifications();
        UINotification::instance.RenderNotifications();

        ImGui::Begin("Console");
        std::lock_guard<std::mutex> guard(mutex);

        // if (ImGui::Button("Clear Console"))
        // {
        //     Clear();
        // }

        ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::TextUnformatted(stdout_buffer.c_str());
        ImGui::TextUnformatted(stderr_buffer.c_str());

        if (shouldScroll)
        {
            ImGui::SetScrollHereY(1.0f);
            shouldScroll = false;
        }
        ImGui::EndChild();

        ImGui::End();

        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        {
            if (ImGui::IsKeyReleased(ImGuiKey_S))
            {
                if (!AppSettings::is_playing)
                {
                    SceneData::save_scene();

                    configs->save_config();

                    string info = "Scene Saved + Game Config: " + configs->current_scene;

                    UINotification::AddNotification(info, 3.0f);
                }
                else
                {
                    string info = "You cannot save scene in playmode";
                    UINotification::AddNotification(info, 3.0f);
                }
            }
        }

        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        {
            if (ImGui::IsKeyReleased(ImGuiKey_D))
            {
                if (select_obj != nullptr)
                {
                    Entity *new_entity = SceneManager::GetOpenScene()->make_entity();

                    new_entity->get_transform()->Position = select_obj->get_transform()->Position;
                    new_entity->get_transform()->rotation = select_obj->get_transform()->rotation;
                    new_entity->get_transform()->Scale = select_obj->get_transform()->Scale;

                    for (const auto &component : select_obj->GetAllComponent())
                    {
                        std::cout << "Cloning: (" << AComponent::demangle(typeid(*component).name()) << ")" << std::endl;
                        Component *cloned_component = ComponentFactory::add_component(new_entity, AComponent::demangle(typeid(*component).name()));

                        cloned_component->variableMap = component->variableMap;
                        cloned_component->init();
                    }

                    set_object_select(new_entity);
                }
            }
        }
    }

    GarinUI::get_ui_manager()->render_ui_context();
}

void gamescene::on_destroy()
{
    configs->save_config();
}

void gamescene::set_object_select(Entity *obj)
{
    select_obj = obj;
    uieditor->select_obj = select_obj;
}