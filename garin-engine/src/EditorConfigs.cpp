#include "../includes/EditorConfigs.h"
#include <GarinGraphics.h>

#include <VarVerify.h>
#include <GarinIO.h>

using namespace nlohmann;

void EditorConfigs::save_config()
{
    json settings;
    settings["current_scene"] = SceneManager::GetOpenScene()->scene_name;
    settings["start_scene"] = start_scene;
    settings["camera_state"] = Graphics::get_main_camera()->use_projection;

    FileManager::write_file(FileManager::get_execute_path() + current_proyect + "/EngineSettings.data", settings.dump(4));
}

void EditorConfigs::load_config()
{
    json settings = json::parse(FileManager::read_file(FileManager::get_execute_path() + current_proyect + "/EngineSettings.data"));
    current_scene = (std::string)settings["current_scene"];
    start_scene = (std::string)settings["start_scene"];
    VarVerify::set_value_if_exists(settings, "camera_state", Graphics::get_main_camera()->use_projection);
}