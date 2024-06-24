#pragma once

#include <GarinNatives.h>
#include "WindowsCompiler.h"
#include "UIAdministrator.h"
#include "DebugGame.h"

#include "EditorGUI.h"
#include "EditorConfigs.h"

#include <GarinBehaviours.h>
#include <GarinMaths.h>
#include <GarinGraphics.h>
#include <GarinEvents.h>
#include <GarinNatives.h>

#include <TerrainGenerator.h>

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

class gamescene : public scenes
{
public:
    Drawer *drawcube;
    Camera *camera;

    Entity *select_obj;
    EditorConfigs *configs = new EditorConfigs();
    GameBehaviourRunner *runner;

    InspectorUI *inspector;
    MainBarUI *mainbar;
    SceneUI *sceneui;
    FilesUI *filesui;
    AssetsUI *assetsui;
    GameSettingsUI *settingsui;
    HierarchyUI *hierarchyui;
    MenuUI *menuui;
    UINotification *notify;

    void embraceTheDarkness();

    void on_start() override;
    void on_update(float delta_time) override;
    void on_draw() override;
    void draw_ui() override;
    void on_destroy() override;
    void set_object_select(Entity *obj);

    std::string demangle(const char *name);
};