#pragma once

#include <GarinNatives.h>
#include "WindowsCompiler.h"
#include "UIEditorManager.h"

#include <GizmosDrawer.h>

#include <GarinComponents.h>
#include <GarinBehaviours.h>
#include <GarinMaths.h>
#include <GarinGraphics.h>
#include <GarinEvents.h>
#include <GarinUI.h>
#include <GizmoModels.h>

#include <IconsManager.h>
#include <Core.h>
#include <mutex>
#include <sstream>

class Drawer;
class Camera;
class Entity;
class EditorConfigs;
class GameBehaviourRunner;
class InspectorUI;
class MainBarUI;
class SceneUI;
class FilesUI;
class AssetsUI;
class GameSettingsUI;
class HierarchyUI;
class MenuUI;
class UINotification;
class EngineHubUI;
class InputSystemUI;
class AnimatorUI;
class GizmosDrawer;
class gamescene;
class UIEditorManager;

class GARINLIBS_API gamescene : public Scene
{
public:
    static gamescene &getInstance()
    {
        static gamescene instance;
        return instance;
    }

    gamescene(const gamescene &) = delete;
    void operator=(const gamescene &) = delete;

    Drawer *drawcube;
    Entity *select_obj;
    EditorConfigs *configs;
    GizmosDrawer *gizmos;
    UIEditorManager *uieditor;
    GizmoModels *gizmo_models;

    std::stringstream buffer_stdout;
    std::stringstream buffer_stderr;
    std::string stdout_buffer;
    std::string stderr_buffer;
    std::streambuf *old_stdout;
    std::streambuf *old_stderr;
    std::mutex mutex;

    bool shouldScroll = false;
    float camera_speed_sens = 30.0f;

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

    void on_start() override;
    void on_update(float delta_time) override;
    void on_draw() override;
    void draw_ui() override;
    void on_destroy() override;
    void on_edition_mode(float delta_time) override;
    void set_object_select(Entity *obj);

    std::string demangle(const char *name);

public:
    gamescene() = default;
    ~gamescene() = default;
};
