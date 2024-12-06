#include "../includes/gamescene.h"
#include <windows.h>
#include <iostream>
#include <SceneManager.h>
#include <AudioManager.h>
#include <GarinUI.h>
#include <GameBehaviour.h>
#include <RenderPipeline.h>
#include <EditorConfigs.h>
#include <filesystem>

namespace fs = std::filesystem;

SceneManager *sceneManager = new SceneManager();
AudioManager *audioManager = new AudioManager();
GameBehaviourFactory *factory_behaviour = new GameBehaviourFactory();
RenderPipeline *piprender = new RenderPipeline();

int start_engine(int argc, char *argv[]);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hWnd = GetConsoleWindow();
    if (hWnd)
    {
        ShowWindow(hWnd, SW_HIDE);
    }

    const char *args[] = {"MantraxEngine", lpCmdLine};
    int argc = sizeof(args) / sizeof(args[0]);

    return start_engine(argc, const_cast<char **>(args));
}

int start_engine(int argc, char *arvg[])
{
    if (argc == 0)
    {
        return -2;
    }

    audioManager->create();
    audioManager->StartSystem();

    sceneManager->create();

    gamescene *scene_game = &gamescene::getInstance();

    scene_game->configs = new EditorConfigs();

    SceneManager::GetSceneManager()->OpenScene = scene_game;

    settings_window window_config = settings_window();
    window_config.window_name = "Mantrax Engine";
    window_config.width = 1920;
    window_config.height = 1080;
    window_config.maximized = true;

    Gfx::create_windows(window_config);

    scene_game->configs->project_select = true;
    fs::path project_path = fs::path(arvg[1]);

    scene_game->configs->current_proyect = project_path.string() + "/";
    FileManager::game_path = scene_game->configs->current_proyect;

    RenderPipeline::init();
    SceneManager::GetOpenScene()->awake();

    scene_game->configs->load_config();

    SceneManager::GetOpenScene()->init();

    while (!Gfx::try_window_close())
    {
        Gfx::poll_events();
        Gfx::timer_control();
        Gfx::process_window_size();

        RenderPipeline::render();

        SceneManager::GetOpenScene()->on_edition_mode(Timer::delta_time);

        Gfx::swap_buffer();
    }

    SceneManager::GetOpenScene()->on_destroy();
    Gfx::clear_graphics();
    sceneManager->release();
    audioManager->release();

    return 0;
}