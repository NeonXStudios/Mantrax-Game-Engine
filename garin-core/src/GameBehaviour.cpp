#include "../includes/GameBehaviour.h"
#include <GarinGraphics.h>

Entity *GameBehaviour::find_object_by_name(string name)
{
    return SceneManager::GetOpenScene()->get_entity_by_name(name);
}

Entity *GameBehaviour::find_object_by_index(int index)
{
    return SceneManager::GetOpenScene()->get_entity_by_index(index);
}
