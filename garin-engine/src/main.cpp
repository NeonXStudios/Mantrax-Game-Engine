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

    game_graphics->render_loop([game_graphics]()
                               {
                                   SceneManager::GetOpenScene()->main_camera->update();

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

    SceneManager::GetOpenScene()->on_destroy();
    sceneManager->release();
    audioManager->release();

    return 0;
}
