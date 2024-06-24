#include "../includes/GameBehaviour.h"
#include <Graphics.h>

Entity *GameBehaviour::find_object_by_name(string name)
{
    return Graphics::get_current_scene()->get_entity_by_name(name);
}

Entity *GameBehaviour::find_object_by_index(int index)
{
    return Graphics::get_current_scene()->get_entity_by_index(index);
}
