#pragma once
#include <GarinCore.h>

using namespace glm;

class gamescene : public scenes
{
public:
    Drawer *drawcube;
    Camera camera;

    UPhysics *world_physics = nullptr;
    Entity *new_obj = nullptr;
    Entity *new_obj2 = nullptr;
    Entity *physic_object = nullptr;
    Entity *light_obj = nullptr;
    Entity *light_obj2 = nullptr;

    UBody *maked_body;
    UBody *maked_body2;

    void on_start() override
    {
        world_physics = new UPhysics();

        DebugGame::add_message("GAME SCENE STARTED", logger);

        // START UI
        GarinUI::make_ui_context();

        Graphics::set_screen(-5);
        camera = Camera();
        main_camera = camera;

        new_obj = make_entity();
        new_obj2 = make_entity();
        light_obj = make_entity();
        light_obj2 = make_entity();
        physic_object = make_entity();

        physic_object->addComponent<ModelComponent>();
        physic_object->get_transform()->Position.z = -40.0f;
        physic_object->get_transform()->Scale = glm::vec3(1.0f, 1.0f, 1.0f);

        new_obj2->addComponent<ModelComponent>();
        new_obj2->get_transform()->Position.z = -50.0f;
        new_obj2->get_transform()->Scale = glm::vec3(55.0f, 55.0f, 0.0f);

        new_obj->addComponent<ModelComponent>();
        new_obj->get_transform()->Position.z = -10.0f;
        new_obj->get_transform()->Position.y = -10.0f;
        new_obj->get_transform()->Scale = glm::vec3(55.0f, 0.0f, 55.0f);

        maked_body = world_physics->make_body();
        maked_body2 = world_physics->make_body();

        maked_body->set_position(physic_object->get_transform()->Position + glm::vec3(0, 5, 0));
        maked_body2->set_position(physic_object->get_transform()->Position + glm::vec3(0, -4, 0));
        maked_body2->collision_type->radius = 3;

        maked_body2->freeze_x = true;
        maked_body2->freeze_y = true;
        maked_body2->freeze_z = true;

        maked_body->collision_type->utype = UPrimitive::SPHERE;
        maked_body2->collision_type->utype = UPrimitive::SPHERE;
    }

    void on_update() override
    {
        physic_object->get_transform()->Position = maked_body->get_position();
        for (Entity *ent : objects_worlds)
        {
            ent->update();
        }

        if (world_physics != nullptr)
        {
            world_physics->step_world(static_cast<float>(1) / 60);
        }

        GarinUI::get_ui_manager()->render_new_frame_ui_context();

        DebugGame::run_debug_console();

        if (ImGui::IsKeyReleased(ImGuiKey_Backslash))
        {
            DebugGame::active_console(!DebugGame::console_is_active());
        }

        GarinUI::get_ui_manager()->render_ui_context();

        float force_multiplier = 0.01f;

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_W) == GLFW_PRESS)
        {
            maked_body->body_position.z -= force_multiplier;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_A) == GLFW_PRESS)
        {
            maked_body->body_position.x -= force_multiplier;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_S) == GLFW_PRESS)
        {
            maked_body->body_position.z += force_multiplier;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_D) == GLFW_PRESS)
        {
            maked_body->body_position.x += force_multiplier;
        }
    }

    void on_destroy() override
    {
        // CODE ON FINISH ENGINE
    }
};
