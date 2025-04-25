#include "../includes/EngineUI.h"

#include <iostream>
#include <cstdarg>
#include <thread>
#include <SceneData.h>
#include <RenderPipeline.h>
#include <GarinIO.h>
#include <PerlinGenerator.h>
#include <ComponentsDrawer.h>
#include <GarinMaths.h>
#include <UIMasterDrawer.h>
#include <ServiceLocator.h>
#include <GLightPoint.h>

void EngineUI::on_awake()
{
    // old_stdout = std::cout.rdbuf(buffer_stdout.rdbuf());
    // old_stderr = std::cerr.rdbuf(buffer_stderr.rdbuf());
    GarinUI::make_ui_context(Gfx::get_game_window());
    watcher = new TimeWatcher();
}

void EngineUI::on_start()
{
    configs->load_config();

    UIMasterDrawer::get_instance().register_uis();
    std::cout << "Work Path: " << FileManager::get_execute_path() << std::endl;

    IconsManager::init();

    string info = "Scene Loaded: " + configs->current_scene;

    UINotification::AddNotification(info, 3.0f);

    assets_registry = new AssetsRegistry("..", 5);

    cube_gizmo = new GizmoCube();
    circle_gizmo = new GizmoCircle();
    sphere_gizmo = new GizmoSphere();
    capsule_gizmo = new GizmoCapsule();
    enhanced_cube = new EnhancedGizmoCube();
    camera_gizmo = new GizmoArrow();
    grid = new GridDrawer(20.0f, 10.0f);
    grid->initialize();
}

void EngineUI::on_edition_mode(float delta_time)
{
    SceneManager* sceneM = ServiceLocator::get<SceneManager>().get();

    sceneM->get_current_scene()->main_camera->update();

    std::string directory_path = FileManager::get_project_path() + "/clscpp";

    watcher->start_file_watcher(directory_path, []() {});

    if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && configs->project_select)
    {
        auto &camera = sceneM->get_current_scene()->main_camera;

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

                float forwardSpeed = (InputSystem::get_axis(GLFW_KEY_W, GLFW_KEY_S) * 10.0f) * delta_time;
                float leftSpeed = (InputSystem::get_axis(GLFW_KEY_A, GLFW_KEY_D) * 10.0f) * delta_time;

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

    for (Entity *entity : sceneM->get_current_scene()->objects_worlds)
    {
        entity->transform_component->update();

        if (entity->hasComponent<GCamera>())
        {
            entity->getComponent<GCamera>().update();
        }

        if (entity->hasComponent<GLightPoint>())
        {
            entity->getComponent<GLightPoint>().update();
        }

        if (entity->hasComponent<GLightSpot>())
        {
            entity->getComponent<GLightSpot>().update();
        }


        if (entity->hasComponent<GLightDirectional>())
        {
            entity->getComponent<GLightDirectional>().update();
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

    std::string window_name = "Mantrax Editor - " + sceneM->get_current_scene()->scene_name;

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
    SceneManager* sceneM = ServiceLocator::get<SceneManager>().get();

    if (select_obj != nullptr && select_obj->hasComponent<GCollision>())
    {
        glm::vec3 convert_gl_to_physx = (std::any_cast<glm::vec3>(select_obj->getComponent<GCollision>().variableMap["boxSize"])) * 2.0f;

        for (GCollision *col : select_obj->getComponents<GCollision>())
        {
            cube_gizmo->render(sceneM->get_current_scene()->main_camera->GetView(),
                               sceneM->get_current_scene()->main_camera->GetProjectionMatrix(),
                               select_obj->get_transform()->get_matrix(), std::any_cast<glm::vec3>(col->variableMap["boxSize"]));
        }
    }

    if (select_obj != nullptr && select_obj->hasComponent<GAudio>())
    {
        glm::vec3 t_min = glm::vec3(std::any_cast<float>(select_obj->getComponent<GAudio>().variableMap["AudioMin"]));
        glm::vec3 t_max = glm::vec3(std::any_cast<float>(select_obj->getComponent<GAudio>().variableMap["AudioMax"]));

        sphere_gizmo->render(sceneM->get_current_scene()->main_camera->GetView(),
                             sceneM->get_current_scene()->main_camera->GetProjectionMatrix(),
                             select_obj->get_transform()->Position, t_max, glm::vec3(0.0f));

        sphere_gizmo->render(sceneM->get_current_scene()->main_camera->GetView(),
                             sceneM->get_current_scene()->main_camera->GetProjectionMatrix(),
                             select_obj->get_transform()->Position, t_min, glm::vec3(0.0f));
    }

    if (select_obj != nullptr && select_obj->hasComponent<GLightPoint>())
    {
        glm::vec3 t_min = glm::vec3(std::any_cast<float>(select_obj->getComponent<GLightPoint>().variableMap["Radius"]));

        sphere_gizmo->render(sceneM->get_current_scene()->main_camera->GetView(),
                             sceneM->get_current_scene()->main_camera->GetProjectionMatrix(),
                             select_obj->get_transform()->Position, t_min, glm::vec3(0.0f));
    }

    if (select_obj != nullptr && select_obj->hasComponent<GCharacter>())
    {
        capsule_gizmo->render(sceneM->get_current_scene()->main_camera->GetView(),
                              sceneM->get_current_scene()->main_camera->GetProjectionMatrix(),
                              select_obj->get_transform()->Position,
                              select_obj->get_transform()->Scale,
                              select_obj->get_transform()->get_euler_angles());
    }

    if (select_obj != nullptr && select_obj->hasComponent<GCamera>() || 
        select_obj != nullptr && select_obj->hasComponent<GLightSpot>() || 
        select_obj != nullptr && select_obj->hasComponent<GLightDirectional>())
    {
        camera_gizmo->render(sceneM->get_current_scene()->main_camera->GetView(),
                             sceneM->get_current_scene()->main_camera->GetProjectionMatrix(),
                              select_obj->get_transform()->get_matrix(),
                              glm::vec3(3.0f));
    }


    if (UIMasterDrawer::get_instance().get_component<SceneView>() != nullptr){
        if (UIMasterDrawer::get_instance().get_component<SceneView>()->found_object != nullptr){
            enhanced_cube->render(sceneM->get_current_scene()->main_camera->GetView(),
                                  sceneM->get_current_scene()->main_camera->GetProjectionMatrix(),
                                  UIMasterDrawer::get_instance().get_component<SceneView>()->found_object->get_transform()->get_matrix(), 
                                  glm::vec3(1.1f), glm::vec3(0.8f));
        }
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    grid->draw(sceneM->get_current_scene()->main_camera->GetProjectionMatrix(),
               sceneM->get_current_scene()->main_camera->GetView(),
            model,
            glm::vec3(0.5f, 0.5f, 0.5f));
}

void EngineUI::draw_ui()
{
    SceneManager* sceneM = ServiceLocator::get<SceneManager>().get();

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
                SceneData::save_scene(sceneM->get_current_scene());
                configs->save_config();

                std::string info = "Scene Saved + Game Config: " + configs->current_scene;
                UINotification::AddNotification(info, 3.0f);
            }
        }

        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        {
            if (ImGui::IsKeyReleased(ImGuiKey_D))
            {
                if (select_obj != nullptr)
                {
                    Entity *new_entity = sceneM->get_current_scene()->make_entity();

                    new_entity->get_transform()->Position = select_obj->get_transform()->Position;
                    new_entity->get_transform()->rotation = select_obj->get_transform()->rotation;
                    new_entity->get_transform()->Scale = select_obj->get_transform()->Scale;

                    for (const auto &component : select_obj->GetAllComponent())
                    {
                        Component *cloned_component = ComponentFactory::add_component(new_entity, ComponentsDrawer::demangle(typeid(*component).name()));

                        cloned_component->variableMap = component->variableMap;
                        cloned_component->init();
                    }

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