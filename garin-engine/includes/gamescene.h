#pragma once

#include <GarinNatives.h>
#include "WindowsCompiler.h"
#include "UIAdministrator.h"

#include "EditorGUI.h"
#include "EditorConfigs.h"
#include <GizmosDrawer.h>

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

class GARINLIBS_API gamescene : public Scene
{
public:
    Drawer *drawcube;
    Camera *camera;

    Entity *select_obj;
    EditorConfigs *configs;

    InspectorUI *inspector;
    MainBarUI *mainbar;
    SceneUI *sceneui;
    FilesUI *filesui;
    AssetsUI *assetsui;
    GameSettingsUI *settingsui;
    HierarchyUI *hierarchyui;
    MenuUI *menuui;
    UINotification *notify;
    IconsManager *icons;
    EngineHubUI *hub;
    InputSystemUI *inputui;
    AnimatorUI *animatorui;
    GizmosDrawer *gizmos;

    std::stringstream buffer_stdout;
    std::stringstream buffer_stderr;
    std::string stdout_buffer;
    std::string stderr_buffer;
    std::streambuf *old_stdout;
    std::streambuf *old_stderr;
    std::mutex mutex;

    bool shouldScroll = false;

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
};