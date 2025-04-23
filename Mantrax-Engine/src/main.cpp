#include "../includes/EngineUI.h"
#include <windows.h>
#include <iostream>
#include <SceneManager.h>
#include <AudioManager.h>
#include <GarinUI.h>
#include <GameBehaviour.h>
#include <RenderPipeline.h>
#include <EditorConfigs.h>
#include <filesystem>
#include <EngineUI.h>
#include <ServiceLocator.h>
#include <EngineStartCore.h>

namespace fs = std::filesystem;

EngineStartCore * engine_m = nullptr;
GameBehaviourFactory *factory_behaviour = new GameBehaviourFactory();
RenderPipeline *piprender = new RenderPipeline();
EngineUI *engine = new EngineUI();

// int start_engine(int argc, char *argv[]);

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
// {
//     HWND hWnd = GetConsoleWindow();
//     if (hWnd)
//     {
//         ShowWindow(hWnd, SW_HIDE);
//     }

//     const char *args[] = {"MantraxEngine", lpCmdLine};
//     int argc = sizeof(args) / sizeof(args[0]);

//     return start_engine(argc, const_cast<char **>(args));
// }

int main(int argc, char *arvg[])
{
    if (argc == 0)
    {
        return -2;
    }
    
    fs::path project_path = fs::path(arvg[1]);
    FileManager::game_path = project_path.string() + "/";

    
    ServiceLocator::provide_new<EngineStartCore>();
    engine_m = ServiceLocator::get<EngineStartCore>().get();
    engine_m->run_engine();

    EngineUI *scene_game = &EngineUI::getInstance();

    scene_game->on_awake();
    scene_game->configs->project_select = argc != 0;
    scene_game->configs->current_proyect = FileManager::game_path;
    scene_game->on_start();

    while (!Gfx::try_window_close())
    {
        Gfx::poll_events();
        Gfx::timer_control();
        Gfx::process_window_size();

        RenderPipeline::find_target_by_id(engine_m->sceneManager->get_current_scene()->main_camera->render_id)->bind_new_render_data("GizmosData",
            [scene_game]()
                            { scene_game->on_draw(); }
        );

        RenderPipeline::render([](){});
        scene_game->on_edition_mode(Timer::delta_time);
        engine_m->sceneManager->on_edition_mode();
        scene_game->draw_ui();

        Gfx::swap_buffer();
    }

    engine_m->shutdown_services();

    return 0;
}