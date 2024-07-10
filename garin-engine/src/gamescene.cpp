#include "../includes/gamescene.h"

#include <iostream>
#include <cstdarg>
#include <thread>
#include <SceneData.h>

void gamescene::on_start()
{
    camera = new Camera();
    main_camera = camera;

    GarinUI::make_ui_context(Graphics::get_game_window());

    configs->load_config();
    SceneData::load_scene(configs->current_scene);

    std::cout << "CARPETA DE EJECUCION: " << FileManager::get_execute_path() << std::endl;

    inspector = new InspectorUI();
    mainbar = new MainBarUI();
    sceneui = new SceneUI();
    filesui = new FilesUI();
    assetsui = new AssetsUI();
    settingsui = new GameSettingsUI();
    hierarchyui = new HierarchyUI();
    menuui = new MenuUI();

    notify = new UINotification();
    IconsManager::init();

    string info = "OPEN FILE: " + configs->current_scene;

    UINotification::AddNotification(info, 3.0f);
}

void gamescene::on_edition_mode(float delta_time)
{
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {
        camera->move_forward(delta_time, InputSystem::get_axis(GLFW_KEY_W, GLFW_KEY_S) * 0.1f);
        camera->move_left(delta_time, InputSystem::get_axis(GLFW_KEY_A, GLFW_KEY_D) * 0.1f);

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_Q) == GLFW_PRESS)
        {
            camera->cameraPosition.y -= 0.1f;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_E) == GLFW_PRESS)
        {
            camera->cameraPosition.y += 0.1f;
        }

        glm::vec3 newOrientation = glm::rotate(camera->Orientation, glm::radians(InputSystem::get_mouse_y()), glm::normalize(glm::cross(camera->Orientation, camera->GetUp())));

        if (abs(glm::angle(newOrientation, camera->GetUp()) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            camera->Orientation = newOrientation;
        }

        camera->Orientation = glm::rotate(camera->Orientation, glm::radians(-InputSystem::get_mouse_x()), camera->GetUp());
    }

    std::string window_name = "Garin Editor - " + SceneManager::GetOpenScene()->scene_name;

    // glfwSetWindowTitle(Graphics::get_game_window(), window_name.c_str());
}

void gamescene::on_update(float delta_time)
{
}

void gamescene::on_draw()
{
}

void gamescene::draw_ui()
{
    GarinUI::get_ui_manager()->render_new_frame_ui_context(true);

    hierarchyui->game = this;

    inspector->draw(select_obj);
    mainbar->draw(select_obj);
    sceneui->draw(select_obj);
    filesui->draw(select_obj, configs);
    assetsui->draw(configs);
    settingsui->draw(configs);
    hierarchyui->draw(select_obj);
    menuui->draw(this);
    // notify->RenderNotifications();
    UINotification::instance.RenderNotifications();

    GarinUI::get_ui_manager()->render_ui_context();
}

void gamescene::on_destroy()
{
    configs->save_config();
}

void gamescene::set_object_select(Entity *obj)
{
    select_obj = obj;
}