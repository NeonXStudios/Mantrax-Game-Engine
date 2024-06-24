#include "EditorConfigs.h"

using namespace nlohmann;

void EditorConfigs::save_config()
{
    json settings;
    settings["current_scene"] = current_scene;
    settings["start_scene"] = start_scene;

    FileManager::write_file(FileManager::get_execute_path() + current_proyect + "/EngineSettings.data", settings.dump(4));
}

void EditorConfigs::load_config()
{
    json settings = json::parse(FileManager::read_file(FileManager::get_execute_path() + current_proyect + "/EngineSettings.data"));
    current_scene = (std::string)settings["current_scene"];
    start_scene = (std::string)settings["start_scene"];
}