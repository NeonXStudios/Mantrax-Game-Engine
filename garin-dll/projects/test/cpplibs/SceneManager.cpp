#include "SceneManager.h"

scenes *SceneManager::current_scene = nullptr;

std::string SceneManager::get_name_scene()
{
    return SceneManager::current_scene->scene_name;
}

void SceneManager::load_scene(std::string scene_name)
{
    SceneManager::current_scene->load_scene(scene_name);
}