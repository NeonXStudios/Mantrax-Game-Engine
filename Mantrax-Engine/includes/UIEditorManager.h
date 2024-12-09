#pragma once

#include <iostream>
#include "UIComponent.h"
#include "EditorGUI.h"
#include "EditorConfigs.h"
#include "UIAdministrator.h"
#include "EngineUI.h"
#include <CodeEditor.h>
#include "UIEditor.h"

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
class GraphicsManagerUI;
class EngineUI;
class CodeEditor;
class MakerUI;
class UIEditor;

class UIEditorManager
{
public:
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
    EditorConfigs *configs;
    Entity *select_obj;
    GraphicsManagerUI *graphicsui;
    MakerUI *makerui;
    UIEditor *ui_editor;

    EngineUI *game;

    void setup();
    void draw();
};
