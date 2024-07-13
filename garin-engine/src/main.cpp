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

    settings_window window_config = settings_window();
    window_config.window_name = "Garin Engine";
    window_config.width = 1920;
    window_config.height = 1080;
    window_config.maximized = true;

    Gfx::create_windows(window_config);

    SceneManager::GetOpenScene()->init();
    std::cout << "Al system started" << std::endl;

    while (!Gfx::try_window_close())
    {
        Gfx::poll_events();
        Gfx::timer_control();
        Gfx::process_window_size();
        Gfx::render_to_texture();

        if (SceneManager::GetOpenScene()->main_camera != nullptr)
        {
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
        }

        Gfx::swap_buffer();
    }

    Gfx::clear_graphics();
    SceneManager::GetOpenScene()->on_destroy();
    sceneManager->release();
    audioManager->release();

    return 0;
}
