#include "../includes/EngineStartCore.h"


void EngineStartCore::run_engine()
{
    ServiceLocator::provide_new<SceneManager>();
    ServiceLocator::provide_new<AudioManager>();
    ServiceLocator::provide_new<RenderPipeline>();

    sceneManager = ServiceLocator::get<SceneManager>().get();
    audioManager = ServiceLocator::get<AudioManager>().get();
    render_pipeline = ServiceLocator::get<RenderPipeline>().get();
    
    audioManager->StartSystem();
    
    Gfx::create_windows();

    render_pipeline->init();

    sceneManager->on_awake();
}

void EngineStartCore::shutdown_services()
{
    sceneManager->on_clean_scenes();
    audioManager->release();
    Gfx::clear_graphics();
}