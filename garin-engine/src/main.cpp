#include "../includes/gamescene.h"
#include <windows.h>
#include <iostream>
#include <SceneManager.h>
#include <AudioManager.h>
#include <GarinUI.h>

SceneManager *sceneManager = new SceneManager();
AudioManager *audioManager = new AudioManager();

int main(void)
{
    // start audio systems
    audioManager->create();
    audioManager->StartSystem();

    // start scene manager
    sceneManager->create();
    gamescene *scene_game = new gamescene();
    SceneManager::GetSceneManager()->OpenScene = scene_game;
    Graphics *game_graphics = new Graphics();
    game_graphics->start_graphics("Garin Editor", 1920, 1080, false, [game_graphics]() {});

<<<<<<< HEAD
    settings_window window_config = settings_window();
    window_config.window_name = "Garin Engine";
    window_config.width = 1920;
    window_config.height = 1080;
    window_config.maximized = true;
    window_config.api_graphic = OpenGL;
=======
    game_graphics->render_loop([game_graphics]()
                               {
                                   SceneManager::GetOpenScene()->main_camera->update();
>>>>>>> parent of 091c1e6 (New GFX render sistem and init graphics)

                                   if (AppSettings::is_playing)
                                   {
                                       SceneManager::GetOpenScene()->update(Timer::delta_time);
                                       game_graphics->runner->on_tick();
                                   }
                                   else
                                   {
                                       SceneManager::GetOpenScene()->on_edition_mode(Timer::delta_time);
                                   } });

    game_graphics->clean_graphics();

<<<<<<< HEAD
    while (!Gfx::try_window_close())
    {
        Gfx::render_to_texture();
        Gfx::poll_events();
        Gfx::timer_control();
        Gfx::process_window_size();
        SceneManager::GetOpenScene()->main_camera->update();

        if (AppSettings::is_playing)
        {
            SceneManager::GetOpenScene()->update(Timer::delta_time);
            // game_graphics->runner->on_tick();
        }
        else
        {
            SceneManager::GetOpenScene()->on_edition_mode(Timer::delta_time);
        }

        Gfx::swap_buffer();
    }

    Gfx::clear_graphics();
=======
>>>>>>> parent of 091c1e6 (New GFX render sistem and init graphics)
    SceneManager::GetOpenScene()->on_destroy();
    sceneManager->release();
    audioManager->release();

    return 0;
}
