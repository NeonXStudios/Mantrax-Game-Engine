#pragma once
#include <GarinNatives.h>
#include "WindowsCompiler.h"

#include <GarinComponents.h>
#include <GarinBehaviours.h>
#include <GarinMaths.h>
#include <GarinGraphics.h>
#include <GarinEvents.h>
#include <GarinUI.h>

#include <IconsManager.h>
#include <Core.h>
#include <mutex>
#include <sstream>
#include <AssetsRegistry.h>
#include <EditorGUI.h>
#include <TimeWatcher.h>
#include <UIBehaviour.h>

#include <GizmoCube.h>
#include <UIMasterDrawer.h>
#include <EditorConfigs.h>

class GARINLIBS_API EngineUI
{
public:
    static EngineUI &getInstance()
    {
        static EngineUI instance;
        return instance;
    }

    EngineUI(const EngineUI &) = delete;
    void operator=(const EngineUI &) = delete;

    Entity *select_obj;
    EditorConfigs *configs;
    AssetsRegistry *assets_registry;
    TimeWatcher *watcher;
    GizmoCube *cube_gizmo;
    GizmoCircle *circle_gizmo;
    GizmoSphere *sphere_gizmo;
    UIBehaviour *ui_behaviour;

    std::stringstream buffer_stdout;
    std::stringstream buffer_stderr;
    std::string stdout_buffer;
    std::string stderr_buffer;
    std::streambuf *old_stdout;
    std::streambuf *old_stderr;
    std::mutex mutex;
    bool first_frame_loaded_on_bucle;

    bool shouldScroll = false;

    float cameraYaw = 0.0f;
    float cameraPitch = 0.0f;

    void ReadBuffer(std::stringstream &buffer, std::string &target)
    {
        std::string line;
        while (std::getline(buffer, line))
        {
            target += line + "\n";
        }
        buffer.clear();
    }

    void Clear()
    {
        std::lock_guard<std::mutex> guard(mutex);
        stdout_buffer.clear();
        stderr_buffer.clear();
    }

    void purpledark();
    void embraceTheDarkness();

    void on_awake();
    void on_start();
    void on_update(float delta_time);
    void on_draw();
    void draw_ui();
    void on_destroy();
    void on_edition_mode(float delta_time);
    void set_object_select(Entity *obj);

    std::string demangle(const char *name);

public:
    EngineUI() = default;
    ~EngineUI() = default;
};
