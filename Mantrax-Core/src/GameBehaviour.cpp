#include "../includes/GameBehaviour.h"
#include <GarinGraphics.h>

Entity *GameBehaviour::find_object_by_name(string name)
{
    return SceneManager::get_open_scene()->get_entity_by_name(name);
}

Entity *GameBehaviour::find_object_by_index(int index)
{
    return SceneManager::get_open_scene()->get_entity_by_index(index);
}
