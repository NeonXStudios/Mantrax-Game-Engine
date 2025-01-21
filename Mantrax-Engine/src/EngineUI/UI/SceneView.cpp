#include <SceneView.h>
#include <iostream>
#include <string>
#include <SceneData.h>
#include <UIAdministrator.h>
#include <array> 

using namespace ImGuizmo;

static ImGuizmo::MODE gizmoMode(ImGuizmo::LOCAL);
static ImGuizmo::OPERATION gizmoOperation(ImGuizmo::TRANSLATE);

void SceneView::on_draw()
{
    ImGui::PushID(343456);

    std::string new_name = "Scene View##" + std::to_string(window_id);

    ImGui::SetNextWindowSizeConstraints(ImVec2(300, 200), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin(new_name.c_str(), &is_open);

    ImGuizmo::SetDrawlist();

    ImVec2 windowSize = ImGui::GetContentRegionAvail();

    ImVec2 p = ImGui::GetCursorScreenPos();
    ImGui::Image((void *)(intptr_t)SceneManager::get_current_scene()->main_camera->render_id, ImVec2(Gfx::render_width, Gfx::render_height), ImVec2(0, 1), ImVec2(1, 0));
    imagePosition = ImGui::GetWindowPos();

    ImVec2 buttonPosition = ImVec2(p.x + 10, p.y + 10);
    ImGui::SetCursorScreenPos(buttonPosition);

    if (ImGui::ImageButton((void *)(intptr_t)IconsManager::MOVE(), ImVec2(24, 24)))
    {
        gizmoOperation = ImGuizmo::TRANSLATE;
    }

    ImGui::SetCursorScreenPos(ImVec2(buttonPosition.x + 50, buttonPosition.y));
    if (ImGui::ImageButton((void *)(intptr_t)IconsManager::ROTATE(), ImVec2(24, 24)))
    {
        gizmoOperation = ImGuizmo::ROTATE;
    }

    ImGui::SetCursorScreenPos(ImVec2(buttonPosition.x + 100, buttonPosition.y));
    if (ImGui::ImageButton((void *)(intptr_t)IconsManager::SCALE(), ImVec2(24, 24)))
    {
        gizmoOperation = ImGuizmo::SCALE;
    }

    ImGui::SetCursorScreenPos(ImVec2(buttonPosition.x + 200, buttonPosition.y));
    if (ImGui::Button("Make New Window Scene"))
    {
        SceneDataView* new_data_scene_View = new SceneDataView();
        new_data_scene_View->work_scene = SceneManager::make_new_empty_scene("Entitie Scene");
        Entity* ent = new_data_scene_View->work_scene->make_entity();

        windows_data.push_back(new_data_scene_View);
    }

    float imageRightX = p.x + Gfx::render_width;
    ImVec2 rightButtonPosition = ImVec2(imageRightX - 50, p.y + 10);
    ImGui::SetCursorScreenPos(rightButtonPosition);

    auto current_camera_type_icon = SceneManager::get_current_scene()->main_camera->use_projection ? IconsManager::PERSPECTIVE() : IconsManager::ORTHO();
    if (ImGui::ImageButton((void *)(intptr_t)current_camera_type_icon, ImVec2(24, 24)))
    {
        SceneManager::get_current_scene()->main_camera->use_projection = !SceneManager::get_current_scene()->main_camera->use_projection;
    }

    Gfx::render_width = windowSize.x; 
    Gfx::render_height = windowSize.y;

    if (ImGui::IsWindowHovered())
    {
        CastData *data = new CastData();

        if (ImGui::IsMouseClicked(0))
        {
            EventSystem::ViewportRenderPosition = glm::vec2(p.x, p.y);
            WorldPoint = EventSystem::screen_to_viewport();

            if (EventSystem::MouseCast2D(WorldPoint, data))
            {
                if (EngineUI::getInstance().select_obj != data->object)
                {
                    EngineUI::getInstance().select_obj = data->object;
                }
            }
            else
            {
                if (!ImGuizmo::IsOver())
                {
                    EngineUI::getInstance().select_obj = nullptr;
                }
            }
        }else{
            EventSystem::ViewportRenderPosition = glm::vec2(p.x, p.y);
            WorldPoint = EventSystem::screen_to_viewport();

            if (EventSystem::MouseCast2D(WorldPoint, data))
            {
                if (found_object != data->object)
                {
                    found_object = data->object;
                }
            }
            else
            {
                if (!ImGuizmo::IsOver())
                {
                    found_object = nullptr;
                }
            }            
        }

        // CastDataUI *data_ui = new CastDataUI();

        // ScreenPoint = EventSystem::mouse_to_screen_pos(glm::vec2(p.x, p.y), glm::vec2(Gfx::render_width, Gfx::render_height));

        // if (ImGui::IsMouseClicked(0))
        // {
        //     if (EventSystem::MouseCastUI(ScreenPoint, data_ui))
        //     {
        //         if (data_ui->object != nullptr)
        //         {
        //             EngineUI::getInstance().ui_behaviour = data_ui->object;
        //         }
        //         else
        //         {
        //             EngineUI::getInstance().ui_behaviour = nullptr;
        //         }
        //     }
        //     else
        //     {
        //         EngineUI::getInstance().ui_behaviour = nullptr;
        //     }
        // }

        if (ImGui::IsMouseDragging(0) && EngineUI::getInstance().ui_behaviour != nullptr)
        {
            EngineUI::getInstance().ui_behaviour->behaviour->Position = glm::vec3(ScreenPoint.x, ScreenPoint.y, EngineUI::getInstance().ui_behaviour->behaviour->Position.z);
        }
    }

    if (EngineUI::getInstance().select_obj != nullptr)
    {
        TransformComponent *transform = EngineUI::getInstance().select_obj->get_transform();

        glm::mat4 matrix = transform->get_matrix();

        glm::mat4 parentMatrix(1.0f);
        if (transform->parent)
        {
            parentMatrix = transform->parent->get_matrix();
        }

        float *projection = (float *)glm::value_ptr(SceneManager::get_current_scene()->main_camera->GetProjectionMatrix());
        float *view = (float *)glm::value_ptr(SceneManager::get_current_scene()->main_camera->GetView());

        ImGuizmo::SetRect(p.x, p.y, Gfx::render_width, Gfx::render_height);

        bool res = ImGuizmo::Manipulate(view, projection, gizmoOperation, gizmoMode, glm::value_ptr(matrix));
        ignoreGui &= !ImGuizmo::IsOver();

        if (res)
        {
            if (transform->parent)
            {
                glm::mat4 localMatrix = glm::inverse(parentMatrix) * matrix;

                glm::vec3 translation = glm::vec3(localMatrix[3]);

                glm::vec3 scale(
                    glm::length(glm::vec3(localMatrix[0])),
                    glm::length(glm::vec3(localMatrix[1])),
                    glm::length(glm::vec3(localMatrix[2])));

                glm::mat3 rotationMat(
                    glm::vec3(localMatrix[0]) / scale.x,
                    glm::vec3(localMatrix[1]) / scale.y,
                    glm::vec3(localMatrix[2]) / scale.z
                    );

                glm::quat rotation = glm::quat_cast(rotationMat);
                glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(rotation));

                for (int i = 0; i < 3; i++)
                {
                    if (std::abs(eulerRotation[i]) < 0.0001f)
                    {
                        eulerRotation[i] = 0.0f;
                    }
                    while (eulerRotation[i] > 180.0f)
                        eulerRotation[i] -= 360.0f;
                    while (eulerRotation[i] < -180.0f)
                        eulerRotation[i] += 360.0f;
                }

                transform->Position = translation;
                transform->Scale = scale;
                transform->set_rotation(eulerRotation);
            }
            else
            {
                glm::vec3 translation = glm::vec3(matrix[3]);

                glm::vec3 scale(
                    glm::length(glm::vec3(matrix[0])),
                    glm::length(glm::vec3(matrix[1])),
                    glm::length(glm::vec3(matrix[2])));

                glm::mat3 rotationMat(
                    glm::vec3(matrix[0]) / scale.x,
                    glm::vec3(matrix[1]) / scale.y,
                    glm::vec3(matrix[2]) / scale.z);

                glm::quat rotation = glm::quat_cast(rotationMat);
                glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(rotation));

                for (int i = 0; i < 3; i++)
                {
                    if (std::abs(eulerRotation[i]) < 0.0001f)
                    {
                        eulerRotation[i] = 0.0f;
                    }
                    while (eulerRotation[i] > 180.0f)
                        eulerRotation[i] -= 360.0f;
                    while (eulerRotation[i] < -180.0f)
                        eulerRotation[i] += 360.0f;
                }

                if (!EngineUI::getInstance().select_obj->hasComponent<GBody>())
                {
                    transform->Position = translation;
                    transform->Scale = scale;
                    transform->set_rotation(eulerRotation);
                }
                else
                {
                    transform->Position = translation;
                    transform->Scale = scale;
                    transform->set_rotation(eulerRotation);

                    EngineUI::getInstance().select_obj->getComponent<GBody>().body->setLinearVelocity(PxVec3(0, 0, 0));
                    EngineUI::getInstance().select_obj->getComponent<GBody>().set_position(translation);
                }
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

    if (ImGui::IsKeyDown(ImGuiKey_Delete) && EngineUI::getInstance().select_obj != nullptr)
    {
        Entity *safe_to_delete = EngineUI::getInstance().select_obj;
        EngineUI::getInstance().select_obj = nullptr;

        SceneManager::get_current_scene()->destroy(safe_to_delete);
    }

    if (ImGui::IsWindowHovered() && ImGui::IsKeyDown(ImGuiKey_F) && EngineUI::getInstance().select_obj != nullptr)
    {
        TransformComponent* selectedObj = EngineUI::getInstance().select_obj->get_transform();
        Camera* camera = SceneManager::get_current_scene()->main_camera;

        glm::vec3 targetPosition = selectedObj->Position;

        glm::vec3 directionToTarget = glm::normalize(targetPosition - camera->cameraPosition);

        EngineUI::yaw = atan2(directionToTarget.x, directionToTarget.z);
        EngineUI::pitch = -asin(directionToTarget.y);

        glm::quat rotationX = glm::angleAxis(EngineUI::pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat rotationY = glm::angleAxis(EngineUI::yaw, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::quat targetRotation = glm::normalize(rotationY * rotationX);

        float distanceToObject = 5.0f;

        camera->cameraPosition = targetPosition - (directionToTarget * distanceToObject);

        if (camera->use_projection)
        {
            camera->cameraRotation = targetRotation;
        }
        else
        {
            camera->cameraRotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    ImGui::End();
    ImGui::PopID();

    ImGui::PushID(554345);
    ImGui::Begin("Game", &is_open);

    ImGuizmo::SetDrawlist();

    Camera* camera_with_max_depth = nullptr;
    float max_depth = -1.0f;

    for (Entity* ent : SceneManager::get_current_scene()->objects_worlds)
    {
        if (ent->hasComponent<GCamera>())
        {
            GCamera& camera_component = ent->getComponent<GCamera>();
            Camera* camera = camera_component.a_camera;

            int current_depth = std::any_cast<int>(camera_component.variableMap["Depth"]);

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
        ImGui::Image((void*)(intptr_t)camera_with_max_depth->render_id, ImVec2(Gfx::render_width, Gfx::render_height), ImVec2(0, 1), ImVec2(1, 0));
    }
    else
    {
        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        ImVec2 textSize = ImGui::CalcTextSize("No valid camera found in the scene.");
        ImVec2 textPos = ImVec2((windowSize.x - textSize.x) * 0.5f, (windowSize.y - textSize.y) * 0.5f);
        ImGui::SetCursorPos(textPos);

        ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0]; 
        ImGui::PushFont(boldFont); 
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10)); 
        ImGui::SetWindowFontScale(2.0f); 

        ImGui::Text("No valid camera found in the scene.");

        ImGui::PopFont();
        ImGui::PopStyleVar();
    }

    ImGui::End();
    ImGui::PopID();

    for (SceneDataView* scene_data_to_render : windows_data)
    {
        scene_data_to_render->draw();
    }
}

SceneDataView::SceneDataView()
{
    window_id = IDGenerator::generate_id();
}

void SceneDataView::draw() {
    ImGui::PushID(window_id);

    std::string unique_window_name = "Scene Window##" + std::to_string(window_id);
    std::string unique_view_name = "View Port##" + std::to_string(window_id);
    std::string unique_entitie_name = "Entitie Details##" + std::to_string(window_id);
    std::string unique_hierarchy_name = "Entities##" + std::to_string(window_id);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

    ImGui::Begin(unique_window_name.c_str(), nullptr);
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {

                if (ImGui::MenuItem("Save")) {
                    SceneData::save_entitie(work_scene);
                }

                if (ImGui::MenuItem("Close")) {
                    close_window();
                }
                
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar(); 
        }

        ImGuiID dockspace_id = ImGui::GetID(unique_window_name.c_str());
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        static bool initialized = false;
        if (!initialized)
        {
            initialized = true;

            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);

            ImGuiID dock_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.25f, nullptr, &dockspace_id);
            ImGuiID dock_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id);
            ImGuiID dock_center = dockspace_id; 

            ImGui::DockBuilderDockWindow(unique_entitie_name.c_str(), dock_right);
            ImGui::DockBuilderDockWindow(unique_hierarchy_name.c_str(), dock_left);
            ImGui::DockBuilderDockWindow(unique_view_name.c_str(), dock_center);

            ImGui::DockBuilderFinish(dockspace_id);
        }

    ImGui::End();

    ImGui::SetNextWindowSizeConstraints(ImVec2(300, 200), ImVec2(FLT_MAX, FLT_MAX));


    ImGuizmo::SetDrawlist();

    ImGui::Begin(unique_view_name.c_str());
    ImVec2 windowSize = ImGui::GetContentRegionAvail();

    ImVec2 p = ImGui::GetCursorScreenPos();

    ImVec2 available_size = ImGui::GetContentRegionAvail();

    ImGui::Image(
        (void *)(intptr_t)work_scene->main_camera->render_id,
        available_size,                                      
        ImVec2(0, 1),                                         
        ImVec2(1, 0)      
    );

    imagePosition = ImGui::GetWindowPos();
    ImGui::End();


    ImVec2 buttonPosition = ImVec2(p.x + 10, p.y + 10);
    ImGui::SetCursorScreenPos(buttonPosition);

    if (ImGui::ImageButton((void *)(intptr_t)IconsManager::MOVE(), ImVec2(24, 24)))
    {
        gizmoOperation = ImGuizmo::TRANSLATE;
    }

    ImGui::SetCursorScreenPos(ImVec2(buttonPosition.x + 50, buttonPosition.y));
    if (ImGui::ImageButton((void *)(intptr_t)IconsManager::ROTATE(), ImVec2(24, 24)))
    {
        gizmoOperation = ImGuizmo::ROTATE;
    }

    ImGui::SetCursorScreenPos(ImVec2(buttonPosition.x + 100, buttonPosition.y));
    if (ImGui::ImageButton((void *)(intptr_t)IconsManager::SCALE(), ImVec2(24, 24)))
    {
        gizmoOperation = ImGuizmo::SCALE;
    }

    float imageRightX = p.x + Gfx::render_width;
    ImVec2 rightButtonPosition = ImVec2(imageRightX - 50, p.y + 10);
    ImGui::SetCursorScreenPos(rightButtonPosition);

    auto current_camera_type_icon = work_scene->main_camera->use_projection ? IconsManager::PERSPECTIVE() : IconsManager::ORTHO();
    if (ImGui::ImageButton((void *)(intptr_t)current_camera_type_icon, ImVec2(24, 24)))
    {
        work_scene->main_camera->use_projection = !work_scene->main_camera->use_projection;
    }

    // Gfx::render_width = windowSize.x; 
    // Gfx::render_height = windowSize.y;

    if (ImGui::IsWindowHovered())
    {
        CastData *data = new CastData();

        if (ImGui::IsMouseClicked(0))
        {
            EventSystem::ViewportRenderPosition = glm::vec2(p.x, p.y);
            WorldPoint = EventSystem::screen_to_viewport();

            if (EventSystem::MouseCast2D(WorldPoint, data))
            {
                if (EngineUI::getInstance().select_obj != data->object)
                {
                    EngineUI::getInstance().select_obj = data->object;
                }
            }
            else
            {
                if (!ImGuizmo::IsOver())
                {
                    EngineUI::getInstance().select_obj = nullptr;
                }
            }
        }

        if (ImGui::IsMouseDragging(0) && EngineUI::getInstance().ui_behaviour != nullptr)
        {
            EngineUI::getInstance().ui_behaviour->behaviour->Position = glm::vec3(ScreenPoint.x, ScreenPoint.y, EngineUI::getInstance().ui_behaviour->behaviour->Position.z);
        }
    }

    if (EngineUI::getInstance().select_obj != nullptr)
    {
        TransformComponent *transform = EngineUI::getInstance().select_obj->get_transform();

        glm::mat4 matrix = transform->get_matrix();

        glm::mat4 parentMatrix(1.0f);
        if (transform->parent)
        {
            parentMatrix = transform->parent->get_matrix();
        }

        float *projection = (float *)glm::value_ptr(work_scene->main_camera->GetProjectionMatrix());
        float *view = (float *)glm::value_ptr(work_scene->main_camera->GetView());

        ImGuizmo::SetRect(p.x, p.y, Gfx::render_width, Gfx::render_height);

        bool res = ImGuizmo::Manipulate(view, projection, gizmoOperation, gizmoMode, glm::value_ptr(matrix));
        ignoreGui &= !ImGuizmo::IsOver();

        if (res)
        {
            if (transform->parent)
            {
                glm::mat4 localMatrix = glm::inverse(parentMatrix) * matrix;

                glm::vec3 translation = glm::vec3(localMatrix[3]);

                glm::vec3 scale(
                    glm::length(glm::vec3(localMatrix[0])),
                    glm::length(glm::vec3(localMatrix[1])),
                    glm::length(glm::vec3(localMatrix[2])));

                glm::mat3 rotationMat(
                    glm::vec3(localMatrix[0]) / scale.x,
                    glm::vec3(localMatrix[1]) / scale.y,
                    glm::vec3(localMatrix[2]) / scale.z);

                glm::quat rotation = glm::quat_cast(rotationMat);
                glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(rotation));

                for (int i = 0; i < 3; i++)
                {
                    if (std::abs(eulerRotation[i]) < 0.0001f)
                    {
                        eulerRotation[i] = 0.0f;
                    }
                    while (eulerRotation[i] > 180.0f)
                        eulerRotation[i] -= 360.0f;
                    while (eulerRotation[i] < -180.0f)
                        eulerRotation[i] += 360.0f;
                }

                transform->Position = translation;
                transform->Scale = scale;
                transform->set_rotation(eulerRotation);
            }
            else
            {
                glm::vec3 translation = glm::vec3(matrix[3]);

                glm::vec3 scale(
                    glm::length(glm::vec3(matrix[0])),
                    glm::length(glm::vec3(matrix[1])),
                    glm::length(glm::vec3(matrix[2])));

                glm::mat3 rotationMat(
                    glm::vec3(matrix[0]) / scale.x,
                    glm::vec3(matrix[1]) / scale.y,
                    glm::vec3(matrix[2]) / scale.z);

                glm::quat rotation = glm::quat_cast(rotationMat);
                glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(rotation));

                for (int i = 0; i < 3; i++)
                {
                    if (std::abs(eulerRotation[i]) < 0.0001f)
                    {
                        eulerRotation[i] = 0.0f;
                    }
                    while (eulerRotation[i] > 180.0f)
                        eulerRotation[i] -= 360.0f;
                    while (eulerRotation[i] < -180.0f)
                        eulerRotation[i] += 360.0f;
                }

                if (!EngineUI::getInstance().select_obj->hasComponent<GBody>())
                {
                    transform->Position = translation;
                    transform->Scale = scale;
                    transform->set_rotation(eulerRotation);
                }
                else
                {
                    transform->Position = translation;
                    transform->Scale = scale;
                    transform->set_rotation(eulerRotation);

                    EngineUI::getInstance().select_obj->getComponent<GBody>().body->setLinearVelocity(PxVec3(0, 0, 0));
                    EngineUI::getInstance().select_obj->getComponent<GBody>().set_position(translation);
                }
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

    if (ImGui::IsKeyDown(ImGuiKey_Delete) && EngineUI::getInstance().select_obj != nullptr)
    {
        Entity *safe_to_delete = EngineUI::getInstance().select_obj;
        EngineUI::getInstance().select_obj = nullptr;

        work_scene->destroy(safe_to_delete);
    }

    if (ImGui::IsWindowHovered() && ImGui::IsKeyDown(ImGuiKey_F) && EngineUI::getInstance().select_obj != nullptr)
    {
        TransformComponent* selectedObj = EngineUI::getInstance().select_obj->get_transform();
        Camera* camera = work_scene->main_camera;

        glm::vec3 targetPosition = selectedObj->Position;

        glm::vec3 directionToTarget = glm::normalize(targetPosition - camera->cameraPosition);

        EngineUI::yaw = atan2(directionToTarget.x, directionToTarget.z);
        EngineUI::pitch = -asin(directionToTarget.y);

        glm::quat rotationX = glm::angleAxis(EngineUI::pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat rotationY = glm::angleAxis(EngineUI::yaw, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::quat targetRotation = glm::normalize(rotationY * rotationX);

        float distanceToObject = 5.0f;

        camera->cameraPosition = targetPosition - (directionToTarget * distanceToObject);

        if (camera->use_projection)
        {
            camera->cameraRotation = targetRotation;
        }
        else
        {
            camera->cameraRotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    hierarchy->window_id = window_id;
    hierarchy->render_scene_hierarchy(work_scene);

    ImGui::SetNextWindowSizeConstraints(ImVec2(300, 200), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin(unique_entitie_name.c_str());
        const std::array<const char *, 20> layerNames = {
        "Background",
        "Parallax 1",
        "Parallax 2",
        "Ground",
        "Decorations",
        "Static Objects",
        "Dynamic Objects",
        "Player",
        "Enemies",
        "NPCs",
        "Items",
        "Projectiles",
        "Particles",
        "Lighting",
        "UI Background",
        "UI Elements",
        "Text",
        "Overlays",
        "Post-Processing",
        "Debug"};


    if (EngineUI::getInstance().select_obj != nullptr)
    {
        {
        ImGui::BeginGroup();
        static char nameBuf[256];
        strcpy(nameBuf, EngineUI::getInstance().select_obj->name_object.c_str());
        
        float availableWidth = ImGui::GetContentRegionAvail().x - 25;
        ImGui::SetNextItemWidth(availableWidth);
        
        if (ImGui::InputText("##Name", nameBuf, sizeof(nameBuf)))
        {
            EngineUI::getInstance().select_obj->name_object = nameBuf;
        }

        ImGui::SameLine(availableWidth + 5);
        
        // Centramos el texto "+" en el botón
        float buttonSize = 20;
        ImVec2 cursorPos = ImGui::GetCursorPos();
        if (ImGui::Button("##AddComponent", ImVec2(buttonSize, buttonSize)))
        {
            ImGui::OpenPopup("AddComponentPopup");
        }
        
        // Calculamos la posición para centrar el "+"
        ImVec2 textSize = ImGui::CalcTextSize("+");
        float textPosX = cursorPos.x + (buttonSize - textSize.x) * 0.5f;
        float textPosY = cursorPos.y + (buttonSize - textSize.y) * 0.5f;
        ImGui::GetWindowDrawList()->AddText(
            ImVec2(ImGui::GetWindowPos().x + textPosX, ImGui::GetWindowPos().y + textPosY),
            ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_Text]),
            "+"
        );
        
        ImGui::EndGroup();
        ImGui::Spacing();
    }

    // Tag y Layer
    {
        // Tag
        ImGui::TextDisabled("Tag");
        static char tagBuf[128];
        strcpy(tagBuf, EngineUI::getInstance().select_obj->object_tag.c_str());
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.45f);
        if (ImGui::InputText("##Tag", tagBuf, sizeof(tagBuf)))
        {
            EngineUI::getInstance().select_obj->object_tag = tagBuf;
        }

        ImGui::SameLine();
        float spacing = ImGui::GetContentRegionAvail().x * 0.1f;
        ImGui::Dummy(ImVec2(spacing, 0));
        ImGui::SameLine();

        // Layer
        ImGui::TextDisabled("Layer");
        int currentLayer = 0;
        for (int i = 0; i < layerNames.size(); ++i)
        {
            if (EngineUI::getInstance().select_obj->Layer == (1 << i))
            {
                currentLayer = i;
                break;
            }
        }
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Combo("##Layer", &currentLayer, layerNames.data(), static_cast<int>(layerNames.size())))
        {
            EngineUI::getInstance().select_obj->Layer = (1 << currentLayer);
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Transform
    {
        ImGui::TextDisabled("TRANSFORM");
        ImGui::Spacing();

        TransformComponent* transform = EngineUI::getInstance().select_obj->get_transform();

        // Position
        ImGui::TextDisabled("Position");
        transform->Position = EditorGUI::Vector3("##Position", transform->Position);
        ImGui::Spacing();

        // Rotation
        ImGui::TextDisabled("Rotation");
        glm::vec3 eulerAngles = transform->euler_rotation;
        eulerAngles = EditorGUI::Vector3("##Rotation", eulerAngles);
        transform->set_rotation(eulerAngles);
        ImGui::Spacing();

        // Scale
        ImGui::TextDisabled("Scale");
        transform->Scale = EditorGUI::Vector3("##Scale", transform->Scale);

        transform->update();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Components
    UIAdministrator::draw_ui(EngineUI::getInstance().select_obj);

     ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopup("AddComponentPopup", ImGuiWindowFlags_NoMove))
    {
        ImGui::Text("Components");
        ImGui::Separator();
        ImGui::Spacing();

        // Lista de componentes con scroll
        ImGui::BeginChild("ComponentsList", ImVec2(600, 400), true);

        if (ImGui::Button("Render Mesh", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<ModelComponent>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Rigid Body", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GBody>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Box Collider", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GCollision>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Material", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GMaterial>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Game Script", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GScript>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Character Controller", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GCharacter>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Audio Source", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GAudio>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Perlin Component", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GNoise>().init();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Animator", ImVec2(-1, 0)))
        {
            EngineUI::getInstance().select_obj->addComponent<GAnimator>().init();
            ImGui::CloseCurrentPopup();
        }
        if (!EngineUI::getInstance().select_obj->hasComponent<GCamera>())
        {
            if (ImGui::Button("Camera", ImVec2(-1, 0)))
            {
                EngineUI::getInstance().select_obj->addComponent<GCamera>().init();
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndChild();
        ImGui::EndPopup();
    }
    }
    ImGui::End();
    ImGui::PopID();
}


void SceneDataView::close_window(){
    auto& windows_data = UIMasterDrawer::get_instance().get_component<SceneView>()->windows_data;
        windows_data.erase(std::remove_if(windows_data.begin(), windows_data.end(),
            [this](SceneDataView* item) { return item->window_id == this->window_id; }),
            windows_data.end());

    SceneManager::close_scene(work_scene);
}