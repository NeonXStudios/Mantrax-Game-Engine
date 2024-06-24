#pragma once
#include <GarinCore.h>

using namespace glm;

class gamescene : public scenes
{

public:
    Drawer *drawcube;
    Camera *camera;

    Entity *new_obj = nullptr;

    void on_start() override
    {
        // CSCompiler *gs = new CSCompiler();
        // gs->start_cs_compilation();

        DebugGame::add_message("GAME SCENE STARTED", DebugGame::logger);

        // START UI
        GarinUI::make_ui_context();

        camera = new Camera();
        main_camera = camera;

        new_obj = make_entity();
        new_obj->addComponent<ModelComponent>();

        new_obj->get_transform()->Scale = glm::vec3(0.3f, 0.3f, 0.3f);
    }


    void on_update() override
    {
        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_W) == GLFW_PRESS)
        {
            camera->cameraPosition.z += 0.1f;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_S) == GLFW_PRESS)
        {
            camera->cameraPosition.z -= 0.1f;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_A) == GLFW_PRESS)
        {
            camera->cameraPosition.x += 0.1f;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_D) == GLFW_PRESS)
        {
            camera->cameraPosition.x -= 0.1f;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_Q) == GLFW_PRESS)
        {
            camera->cameraPosition.y += 0.1f;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_E) == GLFW_PRESS)
        {
            camera->cameraPosition.y -= 0.1f;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            float rotX = 0.0f;
            rotX -= 0.1f;

            glm::quat rotation = glm::angleAxis(glm::radians(-rotX), Graphics::get_main_camera()->GetUp());

            camera->Orientation = rotation * Graphics::get_main_camera()->Orientation;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            float rotX = 0.0f;
            rotX -= 0.1f;
            glm::quat rotation = glm::angleAxis(glm::radians(rotX), Graphics::get_main_camera()->GetUp());

            camera->Orientation = rotation * Graphics::get_main_camera()->Orientation;
        }

        GarinUI::get_ui_manager()->render_new_frame_ui_context();

        DebugGame::run_debug_console();

        if (ImGui::IsKeyDown(ImGuiKey_Backslash))
        {
            DebugGame::active_console(!DebugGame::console_is_active());
        }

        GarinUI::get_ui_manager()->render_ui_context();
    }

    void on_destroy() override
    {
        // CODE ON FINISH ENGINE
    }
};