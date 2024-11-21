#include "../includes/gamescene.h"

#include <iostream>
#include <cstdarg>
#include <thread>
#include <SceneData.h>
#include <RenderPipeline.h>
#include <GarinIO.h>
#include <PerlinGenerator.h>


void gamescene::on_awake(){
    GarinUI::make_ui_context(Gfx::get_game_window());
}

void gamescene::on_start()
{
    old_stdout = std::cout.rdbuf(buffer_stdout.rdbuf());
    old_stderr = std::cerr.rdbuf(buffer_stderr.rdbuf());

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

    assets_registry = new AssetsRegistry("..", 5);

    SceneData::load_scene(configs->current_scene);


    cs->setup_mono();

    cs->start_event();
}

void gamescene::on_edition_mode(float delta_time)
{
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && configs->project_select)
    {
        auto &camera = SceneManager::GetOpenScene()->main_camera;
        if (camera != nullptr)
        {
            float forwardSpeed = InputSystem::get_axis(GLFW_KEY_W, GLFW_KEY_S) * 10.0f * delta_time;
            float leftSpeed = InputSystem::get_axis(GLFW_KEY_A, GLFW_KEY_D) * 10.0f * delta_time;

            camera->move_forward(delta_time, forwardSpeed);
            camera->move_left(delta_time, leftSpeed);

            if (InputSystem::on_key_pressed(GLFW_KEY_Q))
            {
                camera->cameraPosition.y -= 10.0f * delta_time;
            }

            if (InputSystem::on_key_pressed(GLFW_KEY_E))
            {
                camera->cameraPosition.y += 10.0f * delta_time;
            }

            float deltaX = -InputSystem::get_mouse_x() * configs->camera_speed_sens * delta_time;
            float deltaY = -InputSystem::get_mouse_y() * configs->camera_speed_sens * delta_time;

            static float yaw = 0.0f;
            static float pitch = 0.0f;

            yaw += deltaX;
            pitch += deltaY;

            pitch = glm::clamp(pitch, -glm::half_pi<float>(), glm::half_pi<float>());

            glm::quat rotationX = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f));
            glm::quat rotationY = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));

            glm::quat targetRotation = glm::normalize(rotationY * rotationX);

            const float smoothingFactor = 0.1f;
            camera->cameraRotation = glm::slerp(camera->cameraRotation, targetRotation, smoothingFactor);

            camera->cameraRotation = glm::normalize(camera->cameraRotation);
        }
    }

    std::string window_name = "Garin Editor - " + SceneManager::GetOpenScene()->scene_name;

    for (Entity *entity : objects_worlds)
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

    Gfx::change_name(window_name);

    if (configs->project_select == true && !first_frame_loaded_on_bucle)
    {
        assets_registry->directoryPath = configs->current_proyect + "/assets/";
        assets_registry->start();
        first_frame_loaded_on_bucle = true;
    }

    //cs->edition_event();
}

void gamescene::on_update(float delta_time)
{
    // GCastHit *hit = new GCastHit();

    // if (GCaster::LineCast(SceneManager::GetOpenScene()->get_entity_by_index(1)->get_transform()->Position, glm::vec3(0.0f, -1.0f, 0.0f), 100, hit, LAYER_1))
    // {
    //     std::cout << "Casting: " << hit->entity->ObjectName << std::endl;
    // }

    //cs->tick_event();
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

        UINotification::instance.RenderNotifications();

        ImGui::Begin("Console");
        std::lock_guard<std::mutex> guard(mutex);

        if (ImGui::Button("Clear Console"))
        {
            stdout_buffer.clear();
            stderr_buffer.clear();
        }

        ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        std::vector<std::string> console_lines;
        std::istringstream stdout_stream(stdout_buffer);
        std::istringstream stderr_stream(stderr_buffer);
        std::string line;

        while (std::getline(stdout_stream, line))
        {
            console_lines.push_back(line);
        }
        while (std::getline(stderr_stream, line))
        {
            console_lines.push_back(line);
        }

        for (int i = 0; i < console_lines.size(); ++i)
        {
            ImGui::PushID(i);

            ImGui::Columns(2, nullptr, false);
            ImGui::SetColumnWidth(0, 20);

            // if (ImGui::Button("X"))
            // {
            //     console_lines.erase(console_lines.begin() + i);
            //     i--;

            //     stdout_buffer.clear();
            //     stderr_buffer.clear();
            //     for (const auto &msg : console_lines)
            //     {
            //         stdout_buffer += msg + "\n";
            //     }
            // }

            ImGui::NextColumn();

            ImGui::TextWrapped(console_lines[i].c_str());

            ImGui::Columns(1);
            ImGui::PopID();
        }

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

        //cs->ui_event();
    }

    GarinUI::get_ui_manager()->render_ui_context();
}

void gamescene::on_destroy()
{
    //cs->release_mono();
    assets_registry->stop();
    configs->save_config();
}

void gamescene::set_object_select(Entity *obj)
{
    select_obj = obj;
    uieditor->select_obj = select_obj;
}