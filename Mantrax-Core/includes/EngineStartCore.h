#pragma once
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
#include <Core.h>

class GARINLIBS_API EngineStartCore
{
public:
    SceneManager *sceneManager = nullptr;
    AudioManager *audioManager = nullptr;
    RenderPipeline *render_pipeline = nullptr;

    void run_engine();
    void render_engine();
    void shutdown_services();
};