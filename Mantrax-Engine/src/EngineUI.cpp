#include "../includes/EngineUI.h"

#include <iostream>
#include <cstdarg>
#include <thread>
#include <SceneData.h>
#include <RenderPipeline.h>
#include <GarinIO.h>
#include <PerlinGenerator.h>

void EngineUI::on_awake()
{
    // old_stdout = std::cout.rdbuf(buffer_stdout.rdbuf());
    // old_stderr = std::cerr.rdbuf(buffer_stderr.rdbuf());
    GarinUI::make_ui_context(Gfx::get_game_window());
    watcher = new TimeWatcher();
}

void EngineUI::on_start()
{

    UIMasterDrawer::get_instance().register_uis();
    std::cout << "CARPETA DE EJECUCION: " << FileManager::get_execute_path() << std::endl;

    IconsManager::init();

    string info = "OPEN FILE: " + configs->current_scene;

    UINotification::AddNotification(info, 3.0f);

    assets_registry = new AssetsRegistry("..", 5);

    cube_gizmo = new GizmoCube();
    circle_gizmo = new GizmoCircle();
    sphere_gizmo = new GizmoSphere();
}

void EngineUI::on_edition_mode(float delta_time)
{
    std::string directory_path = FileManager::get_game_path() + "/clscpp";

    watcher->start_file_watcher(directory_path, []() {});

    if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && configs->project_select)
    {
        auto &camera = SceneManager::get_open_scene()->main_camera;

        if (camera != nullptr)
        {
            float deltaX = -InputSystem::get_mouse_x() * configs->camera_speed_sens * delta_time;
            float deltaY = -InputSystem::get_mouse_y() * configs->camera_speed_sens * delta_time;

            if (camera->use_projection)
            {
                if (InputSystem::on_key_pressed(GLFW_KEY_Q))
                {
                    camera->cameraPosition.y -= 10.0f * delta_time;
                }

                if (InputSystem::on_key_pressed(GLFW_KEY_E))
                {
                    camera->cameraPosition.y += 10.0f * delta_time;
                }

                float forwardSpeed = InputSystem::get_axis(GLFW_KEY_W, GLFW_KEY_S) * 10.0f * delta_time;
                float leftSpeed = InputSystem::get_axis(GLFW_KEY_A, GLFW_KEY_D) * 10.0f * delta_time;

                camera->move_forward(delta_time, forwardSpeed);
                camera->move_left(delta_time, leftSpeed);
            }
            else
            {
                if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
                {
                    camera->cameraPosition.y -= deltaY;
                    camera->move_left(delta_time, deltaX);
                }
            }

            static float yaw = 0.0f;
            static float pitch = 0.0f;

            yaw += deltaX;
            pitch += deltaY;

            pitch = glm::clamp(pitch, -glm::half_pi<float>(), glm::half_pi<float>());

            glm::quat rotationX = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f));
            glm::quat rotationY = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));

            glm::quat targetRotation = glm::normalize(rotationY * rotationX);

            const float smoothingFactor = 0.1f;

            if (camera->use_projection)
            {
                camera->cameraRotation = glm::slerp(camera->cameraRotation, targetRotation, smoothingFactor);
                camera->cameraRotation = glm::normalize(camera->cameraRotation);
            }
            else
            {
                camera->cameraRotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
            }
        }
    }

    for (Entity *entity : SceneManager::get_current_scene()->objects_worlds)
    {
        entity->transform_component->update();

        if (entity->hasComponent<GCamera>())
        {
            entity->getComponent<GCamera>().update();
        }

        for (Entity *transform : entity->childrens)
        {
            transform->transform_component->update();

            if (transform->hasComponent<GCamera>())
            {
                transform->getComponent<GCamera>().update();
            }
        }
    }

    std::string window_name = "Mantrax Editor - " + SceneManager::get_open_scene()->scene_name;

    Gfx::change_name(window_name);

    if (configs->project_select == true && !first_frame_loaded_on_bucle)
    {
        assets_registry->directoryPath = configs->current_proyect + "/assets/";
        assets_registry->start();
        first_frame_loaded_on_bucle = true;
    }
}

void EngineUI::on_update(float delta_time)
{
}

void EngineUI::on_draw()
{
    // if (select_obj != nullptr && select_obj->hasComponent<GCollision>())
    // {
    //     cube_gizmo->render(SceneManager::GetOpenScene()->main_camera->GetView(),
    //                        SceneManager::GetOpenScene()->main_camera->GetProjectionMatrix(),
    //                        select_obj->get_transform()->Position, select_obj->get_transform()->Scale * std::any_cast<glm::vec3>(select_obj->getComponent<GCollision>().variableMap["boxSize"]));
    // }
    // if (select_obj != nullptr && select_obj->hasComponent<GAudio>())
    // {
    //     glm::vec3 t_min = glm::vec3(std::any_cast<float>(select_obj->getComponent<GAudio>().variableMap["AudioMin"]));
    //     glm::vec3 t_max = glm::vec3(std::any_cast<float>(select_obj->getComponent<GAudio>().variableMap["AudioMax"]));

    //     sphere_gizmo->render(SceneManager::GetOpenScene()->main_camera->GetView(),
    //                          SceneManager::GetOpenScene()->main_camera->GetProjectionMatrix(),
    //                          select_obj->get_transform()->Position, t_max, glm::vec3(0.0f));

    //     sphere_gizmo->render(SceneManager::GetOpenScene()->main_camera->GetView(),
    //                          SceneManager::GetOpenScene()->main_camera->GetProjectionMatrix(),
    //                          select_obj->get_transform()->Position, t_min, glm::vec3(0.0f));
    // }
}

void EngineUI::draw_ui()
{
    GarinUI::get_ui_manager()->render_new_frame_ui_context(true);

    if (configs->project_select == true)
    {
        // ReadBuffer(buffer_stdout, stdout_buffer);
        // ReadBuffer(buffer_stderr, stderr_buffer);

        UINotification::instance.RenderNotifications();

        // ImGui::Begin("Console");
        // std::lock_guard<std::mutex> guard(mutex);

        // if (ImGui::Button("Clear Console"))
        // {
        //     stdout_buffer.clear();
        //     stderr_buffer.clear();
        // }

        // ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        // std::vector<std::string> console_lines;
        // std::istringstream stdout_stream(stdout_buffer);
        // std::istringstream stderr_stream(stderr_buffer);
        // std::string line;

        // while (std::getline(stdout_stream, line))
        // {
        //     console_lines.push_back(line);
        // }
        // while (std::getline(stderr_stream, line))
        // {
        //     console_lines.push_back(line);
        // }

        // for (int i = 0; i < console_lines.size(); ++i)
        // {
        //     ImGui::PushID(i);

        //     ImGui::Columns(2, nullptr, false);
        //     ImGui::SetColumnWidth(0, 20);

        //     ImGui::NextColumn();

        //     ImGui::TextWrapped(console_lines[i].c_str());

        //     ImGui::Columns(1);
        //     ImGui::PopID();
        // }

        // if (shouldScroll)
        // {
        //     ImGui::SetScrollHereY(1.0f);
        //     shouldScroll = false;
        // }

        // ImGui::EndChild();
        // ImGui::End();

        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        {
            if (ImGui::IsKeyReleased(ImGuiKey_S))
            {
                if (!AppSettings::is_playing)
                {
                    std::string bg_pick_path = configs->current_proyect + "/gb.jpg";
                    GarinIO::screenshot(Gfx::main_render->get_render(), Gfx::width, Gfx::height, bg_pick_path.c_str());

                    SceneData::save_scene();
                    configs->save_config();

                    std::string info = "Scene Saved + Game Config: " + configs->current_scene;
                    UINotification::AddNotification(info, 3.0f);
                }
                else
                {
                    std::string info = "You cannot save scene in playmode";
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
                    Entity *new_entity = SceneManager::get_open_scene()->make_entity();

                    new_entity->get_transform()->Position = select_obj->get_transform()->Position;
                    new_entity->get_transform()->rotation = select_obj->get_transform()->rotation;
                    new_entity->get_transform()->Scale = select_obj->get_transform()->Scale;

                    // for (const auto &component : select_obj->GetAllComponent())
                    // {
                    //     Component *cloned_component = ComponentFactory::add_component(new_entity, AComponent::demangle(typeid(*component).name()));

                    //     cloned_component->variableMap = component->variableMap;
                    //     cloned_component->init();
                    // }

                    set_object_select(new_entity);
                }
            }
        }
    }

    UIMasterDrawer::get_instance().on_draw();

    GarinUI::get_ui_manager()->render_ui_context();
}

void EngineUI::on_destroy()
{
    assets_registry->stop();
    configs->save_config();
}

void EngineUI::set_object_select(Entity *obj)
{
    select_obj = obj;
}