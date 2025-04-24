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
    EngineStartCore * engine_m = new EngineStartCore();
    GameBehaviourFactory *factory_behaviour = new GameBehaviourFactory();
    ServiceLocator::provide_new<EngineUI>();

    if (argc == 0)
    {
        return -2;
    }
    
    fs::path project_path = fs::path(arvg[1]);
    FileManager::game_path = project_path.string() + "/";

    
    engine_m->run_engine();

    EngineUI *editor_ui = ServiceLocator::get<EngineUI>().get();

    editor_ui->on_awake();
    editor_ui->on_start();

    while (!Gfx::try_window_close())
    {
        Gfx::poll_events();
        Gfx::timer_control();
        Gfx::process_window_size();

        engine_m->render_pipeline->find_target_by_id(
            engine_m->sceneManager->get_current_scene()->main_camera->render_id)->bind_new_render_data("GizmosData", [editor_ui]()
                            { 
                                editor_ui->on_draw(); 
                            }
        );

        engine_m->render_pipeline->render([](){});
        engine_m->sceneManager->on_edition_mode();
        editor_ui->on_edition_mode(Timer::delta_time);
        editor_ui->draw_ui();

        Gfx::swap_buffer();
    }

    engine_m->shutdown_services();

    return 0;
}