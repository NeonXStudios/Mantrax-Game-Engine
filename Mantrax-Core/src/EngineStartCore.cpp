#include "../includes/EngineStartCore.h"


void EngineStartCore::run_engine()
{
    ServiceLocator::provide_new<SceneManager>();
    ServiceLocator::provide_new<AudioManager>();
    ServiceLocator::provide_new<RenderPipeline>();
    ServiceLocator::provide_new<MaterialService>();

    sceneManager = ServiceLocator::get<SceneManager>().get();
    audioManager = ServiceLocator::get<AudioManager>().get();
    render_pipeline = ServiceLocator::get<RenderPipeline>().get();
    material_manager = ServiceLocator::get<MaterialService>().get();

    Gfx::create_windows();
    audioManager->StartSystem();
    render_pipeline->init();
    material_manager->start_services();
    sceneManager->on_awake();
}

void EngineStartCore::shutdown_services()
{
    sceneManager->on_clean_scenes();
    audioManager->release();
    Gfx::clear_graphics();
}