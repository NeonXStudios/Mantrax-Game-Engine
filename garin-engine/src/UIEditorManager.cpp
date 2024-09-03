#include "../includes/UIEditorManager.h"

#include "InspectorUI.h"
#include "MainBarUI.h"
#include "SceneUI.h"
#include "FilesUI.h"
#include "AssetsUI.h"
#include "GameSettingsUI.h"
#include "HierarchyUI.h"
#include "MenuUI.h"
#include "UINotification.h"
#include "EngineHubUI.h"
#include "InputSystemUI.h"
#include "AnimatorUI.h"
#include "EditorConfigs.h"
#include "GraphicsManagerUI.h"
#include "gamescene.h"

void UIEditorManager::setup()
{
    inspector = new InspectorUI();
    mainbar = new MainBarUI();
    sceneui = new SceneUI();
    filesui = new FilesUI();
    assetsui = new AssetsUI();
    settingsui = new GameSettingsUI();
    hierarchyui = new HierarchyUI();
    menuui = new MenuUI();
    hub = new EngineHubUI();
    inputui = new InputSystemUI();
    animatorui = new AnimatorUI();
    graphicsui = new GraphicsManagerUI();

    hub->configs = configs;
    mainbar->configs = configs;

    sceneui->game = game;
    mainbar->game = game;

    filesui->is_open = false;
    settingsui->is_open = false;
    graphicsui->is_open = false;
    inputui->is_open = false;

    notify = new UINotification();
}

void UIEditorManager::draw()
{
    if (configs->project_select)
    {
        if (hierarchyui->is_open)
            hierarchyui->game = game;

        if (mainbar->is_open)
            mainbar->draw(select_obj);

        // if (inspector->is_open)
        inspector->draw(select_obj);

        if (sceneui->is_open)
            sceneui->draw(select_obj);

        if (filesui->is_open)
            filesui->draw(select_obj, configs);

        if (assetsui->is_open)
            assetsui->draw(configs);

        if (settingsui->is_open)
            settingsui->draw(configs);

        if (hierarchyui->is_open)
            hierarchyui->draw(select_obj);

        if (inputui->is_open)
            inputui->draw();

        if (animatorui->is_open)
            animatorui->draw();

        if (graphicsui->is_open)
            graphicsui->draw(configs);

        CodeEditor::get_instance().render_editors();
    }
    else
    {
        hub->draw();
    }
}
