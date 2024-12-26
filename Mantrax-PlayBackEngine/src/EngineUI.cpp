#include "../includes/EngineUI.h"

#include <iostream>
#include <cstdarg>
#include <thread>
#include <RenderPipeline.h>
#include <GarinIO.h>
#include <PerlinGenerator.h>

void EngineUI::on_awake()
{
    GarinUI::make_ui_context(Gfx::get_game_window());
}

void EngineUI::on_start()
{
    IconsManager::init();
    SceneManager::load_scene(configs->current_scene);
}

void EngineUI::on_edition_mode(float delta_time)
{
}

void EngineUI::on_update(float delta_time)
{
}

void EngineUI::on_draw()
{
}

void EngineUI::draw_ui()
{
    GarinUI::get_ui_manager()->render_new_frame_ui_context(false);

    GarinUI::get_ui_manager()->render_ui_context();
}

void EngineUI::on_destroy()
{
}

void EngineUI::set_object_select(Entity *obj)
{
}