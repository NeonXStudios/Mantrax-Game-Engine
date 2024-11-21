#include "../includes/gamescene.h"

#include <iostream>
#include <cstdarg>
#include <thread>
#include <RenderPipeline.h>
#include <GarinIO.h>
#include <PerlinGenerator.h>

void gamescene::on_awake (){
        GarinUI::make_ui_context(Gfx::get_game_window());
}

void gamescene::on_start()
{
    IconsManager::init();

    SceneManager::load_scene(configs->current_scene);

    // cs->setup_mono();
    // cs->start_event();
}

void gamescene::on_edition_mode(float delta_time)
{
    //cs->edition_event();
}

void gamescene::on_update(float delta_time)
{
    //cs->tick_event();
}

void gamescene::on_draw()
{

}

void gamescene::draw_ui()
{
   GarinUI::get_ui_manager()->render_new_frame_ui_context(false);
   
    if (ImGui::Button("Start Game")){
        AppSettings::is_playing = true;
        SceneManager::load_scene(SceneManager::GetOpenScene()->scene_name);
    }

   GarinUI::get_ui_manager()->render_ui_context();
}

void gamescene::on_destroy()
{
    //cs->release_mono();
}

void gamescene::set_object_select(Entity *obj)
{
}