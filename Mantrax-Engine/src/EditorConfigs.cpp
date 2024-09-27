#include "../includes/EditorConfigs.h"
#include <GarinGraphics.h>

#include <VarVerify.h>
#include <GarinIO.h>
#include <RenderPipeline.h>

using namespace nlohmann;

void EditorConfigs::save_config()
{
    json settings;
    settings["current_scene"] = SceneManager::GetOpenScene()->scene_name;
    settings["start_scene"] = start_scene;
    settings["camera_state"] = SceneManager::GetOpenScene()->main_camera->use_projection;
    settings["camera_sens"] = camera_speed_sens;

    settings["settings_render"] = RenderPipeline::layers_to_render;

    FileManager::write_file(current_proyect + "/GameSettings.data", settings.dump(4));
}

void EditorConfigs::load_config()
{
    json settings = json::parse(FileManager::read_file(current_proyect + "/GameSettings.data"));

    VarVerify::set_value_if_exists(settings, "current_scene", current_scene);
    VarVerify::set_value_if_exists(settings, "start_scene", start_scene);
    VarVerify::set_value_if_exists(settings, "camera_sens", camera_speed_sens);

    if (SceneManager::GetOpenScene()->main_camera == nullptr)
    {
        std::cout << "Main camera its null" << std::endl;
        return;
    }
    else
    {
        std::cout << "Loading camera config" << std::endl;
    }

    RenderPipeline::layers_to_render.clear();

    for (const auto &item : settings["settings_render"])
    {
        RenderPipeline::layers_to_render.insert(item.get<int>());
    }

    VarVerify::set_value_if_exists(settings, "camera_state", SceneManager::GetOpenScene()->main_camera->use_projection);
}