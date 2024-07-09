#include "../includes/GameBehaviour.h"
#include <GarinGraphics.h>
#include <SceneManager.h>

Entity *GameBehaviour::find_object_by_name(string name)
{
    return SceneManager::current_scene->get_entity_by_name(name);
}

Entity *GameBehaviour::find_object_by_index(int index)
{
    return SceneManager::current_scene->get_entity_by_index(index);
}
