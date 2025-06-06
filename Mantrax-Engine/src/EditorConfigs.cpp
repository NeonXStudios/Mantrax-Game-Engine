#include "../includes/EditorConfigs.h"
#include <GarinGraphics.h>

#include <VarVerify.h>
#include <GarinIO.h>
#include <RenderPipeline.h>
#include <UIStyle.h>
#include <FileManager.h>
#include <ServiceLocator.h>

using namespace nlohmann;

void EditorConfigs::save_config()
{
    SceneManager* sceneM = ServiceLocator::get<SceneManager>().get();
    RenderPipeline* render_pipeline = ServiceLocator::get<RenderPipeline>().get();

    json settings;

    settings["current_scene"] = sceneM->get_current_scene()->scene_name;
    settings["camera_state"] = sceneM->get_current_scene()->main_camera->use_projection;
    settings["settings_render"] = render_pipeline->layers_to_render;

    settings["start_scene"] = start_scene;
    settings["camera_sens"] = camera_speed_sens;
    settings["engine_theme"] = current_theme;

    FileManager::write_file(current_proyect + "/GameSettings.data", settings.dump(4));
}

void EditorConfigs::load_config()
{
    SceneManager* sceneM = ServiceLocator::get<SceneManager>().get();
    RenderPipeline* render_pipeline = ServiceLocator::get<RenderPipeline>().get();
    current_proyect = FileManager::game_path;
    project_select = current_proyect != ""; 
    json settings = json::parse(FileManager::read_file(current_proyect + "/GameSettings.data"));

    VarVerify::set_value_if_exists(settings, "current_scene", current_scene);
    VarVerify::set_value_if_exists(settings, "start_scene", start_scene);
    VarVerify::set_value_if_exists(settings, "camera_sens", camera_speed_sens);
    VarVerify::set_value_if_exists(settings, "engine_theme", current_theme);

    if (current_theme == "DefaultClassic")
    {
        UIStyle::SetStyleUI(DefaultClassic);
    }
    else if (current_theme == "DefaultWhite")
    {
        UIStyle::SetStyleUI(DefaultWhite);
    }
    else if (current_theme == "DefaultDark")
    {
        UIStyle::SetStyleUI(DefaultDark);
    }
    else if (current_theme == "Darkness")
    {
        UIStyle::SetStyleUI(Darkness);
    }
    else if (current_theme == "Dracula")
    {
        UIStyle::SetStyleUI(Dracula);
    }
    else if (current_theme == "RedDark")
    {
        UIStyle::SetStyleUI(RedDark);
    }
    else if (current_theme == "Dark")
    {
        UIStyle::SetStyleUI(Dark);
    }
    else if (current_theme == "Grey")
    {
        UIStyle::SetStyleUI(Grey);
    }
    else if (current_theme == "WhiteY")
    {
        UIStyle::SetStyleUI(WhiteY);
    }
    else if (current_theme == "DarkGrey")
    {
        UIStyle::SetStyleUI(DarkGrey);
    }
    else if (current_theme == "Black")
    {
        UIStyle::SetStyleUI(Black);
    }
    else if (current_theme == "DarkRounded")
    {
        UIStyle::SetStyleUI(DarkRounded);
    }

    if (sceneM->get_current_scene()->main_camera == nullptr)
    {
        std::cout << "Main camera its null" << std::endl;
        return;
    }
    else
    {
        std::cout << "Loading camera config" << std::endl;
    }

    render_pipeline->layers_to_render.clear();

    for (const auto &item : settings["settings_render"])
    {
        render_pipeline->layers_to_render.insert(item.get<int>());
    }

    VarVerify::set_value_if_exists(settings, "camera_state", sceneM->get_current_scene()->main_camera->use_projection);
}