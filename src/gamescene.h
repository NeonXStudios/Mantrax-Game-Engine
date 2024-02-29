#pragma once
#include <GarinCore.h>

using namespace glm;

class gamescene : public scenes
{
public:
    Drawer *drawcube;
    Camera camera;

    Entity *new_obj = nullptr;
    Entity *new_obj2 = nullptr;
    Entity *light_obj = nullptr;
    Entity *light_obj2 = nullptr;

    void on_start() override
    {
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

        new_obj2->addComponent<ModelComponent>();
        new_obj2->get_transform()->Position.z = -50.0f;
        new_obj2->get_transform()->Scale = glm::vec3(55.0f, 55.0f, 0.0f);

        new_obj->addComponent<ModelComponent>();
        new_obj->get_transform()->Position.z = -10.0f;
        new_obj->get_transform()->Position.y = -10.0f;
        new_obj->get_transform()->Scale = glm::vec3(55.0f, 0.0f, 55.0f);

        light_obj->addComponent<ModelComponent>();
        light_obj->addComponent<LightComponent>();
        light_obj->getComponent<LightComponent>().type_light = Spot;
        light_obj->get_transform()->Position = glm::vec3(10.0f, 0.0f, -20.0f);

        // light_obj2->addComponent<ModelComponent>();
        //  light_obj2->addComponent<LightComponent>();
        //  light_obj2->getComponent<LightComponent>().type_light = Point;
        //  light_obj2->get_transform()->Position = glm::vec3(-0.0f, 0.0f, -20.0f);
    }

    void on_update() override
    {
        for (Entity *ent : objects_worlds)
        {
            ent->update();
        }

        GarinUI::get_ui_manager()->render_new_frame_ui_context();
        // GarinUI::slider("LIGHT OFFSET X", glm::vec2(-100, 100), &light_obj->get_transform()->Position.x);
        // GarinUI::slider("LIGHT OFFSET Y", glm::vec2(-100, 100), &light_obj->get_transform()->Position.y);
        // GarinUI::slider("LIGHT OFFSET Z", glm::vec2(-100, 100), &light_obj->get_transform()->Position.z);
        // GarinUI::slider("LIGHT DIFFUSE", glm::vec2(-200, 200), &light_obj->getComponent<LightComponent>().sun_diffuse);
        // GarinUI::slider("LIGHT INTENSITY", glm::vec2(-200, 200), &light_obj->getComponent<LightComponent>().sun_intensity);
        // GarinUI::slider("LIGHT RADIUS", glm::vec2(0, 50), &light_obj->getComponent<LightComponent>().light_radius);

        // GarinUI::slider("LIGHT CONSTANT", glm::vec2(0, 1), &light_obj->getComponent<LightComponent>().constant);
        // GarinUI::slider("LIGHT LINEAR", glm::vec2(0, 1), &light_obj->getComponent<LightComponent>().linear);
        // GarinUI::slider("LIGHT QUADRATIC", glm::vec2(0, 1), &light_obj->getComponent<LightComponent>().quadratic);

        // ImGui::Spacing();

        // GarinUI::slider("LIGHT OFFSET X 2", glm::vec2(-100, 100), &light_obj2->get_transform()->Position.x);
        // GarinUI::slider("LIGHT OFFSET Y 2", glm::vec2(-100, 100), &light_obj2->get_transform()->Position.y);
        // GarinUI::slider("LIGHT OFFSET Z 2", glm::vec2(-100, 100), &light_obj2->get_transform()->Position.z);
        // GarinUI::slider("LIGHT INTENSITY 2", glm::vec2(-200, 200), &light_obj2->getComponent<LightComponent>().sun_intensity);
        // GarinUI::slider("LIGHT RADIUS 2", glm::vec2(0, 50), &light_obj2->getComponent<LightComponent>().light_radius);

        // GarinUI::slider("LIGHT CONSTANT 2", glm::vec2(0, 1), &light_obj2->getComponent<LightComponent>().constant);
        // GarinUI::slider("LIGHT LINEAR 2", glm::vec2(0, 1), &light_obj2->getComponent<LightComponent>().linear);
        // GarinUI::slider("LIGHT QUADRATIC 2", glm::vec2(0, 1), &light_obj2->getComponent<LightComponent>().quadratic);
        // GarinUI::slider("LIGHT SPOT CUTOFF", glm::vec2(0, 160), &light_obj2->getComponent<LightComponent>().cut_off);
        // GarinUI::slider("LIGHT SPOT CUT", glm::vec2(0, 160), &light_obj2->getComponent<LightComponent>().other_cut_off);

        // if (GarinUI::button("Set Spot Light", glm::vec2(200, 50)))
        // {
        //     light_obj->getComponent<LightComponent>().type_light = Spot;
        // }

        // if (GarinUI::button("Set Point Light", glm::vec2(200, 50)))
        // {
        //     light_obj->getComponent<LightComponent>().type_light = Point;
        // }

        // if (GarinUI::button("Set Directional Light", glm::vec2(200, 50)))
        // {
        //     light_obj->getComponent<LightComponent>().type_light = Directional;
        // }

        DebugGame::run_debug_console();

        if (ImGui::IsKeyReleased(ImGuiKey_Backslash))
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
