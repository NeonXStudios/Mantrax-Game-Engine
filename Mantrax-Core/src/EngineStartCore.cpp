#include "../includes/EngineStartCore.h"


void EngineStartCore::run_engine()
{
    
    ServiceLocator::provide_new<SceneManager>();
    sceneManager = ServiceLocator::get<SceneManager>().get();

    ServiceLocator::provide_new<AudioManager>();
    audioManager = ServiceLocator::get<AudioManager>().get();
    audioManager->StartSystem();
    
    Gfx::create_windows();
    RenderPipeline::init();

    sceneManager->on_awake();
}

void EngineStartCore::shutdown_services()
{
    sceneManager->on_clean_scenes();
    audioManager->release();
    Gfx::clear_graphics();
}