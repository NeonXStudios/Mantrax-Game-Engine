#include "../includes/GameBehaviour.h"
#include <GarinGraphics.h>
#include <ServiceLocator.h>

Entity *GameBehaviour::find_object_by_name(string name)
{
    SceneManager* sceneM = ServiceLocator::get<SceneManager>().get();

    return sceneM->get_current_scene()->get_entity_by_name(name);
}

Entity *GameBehaviour::find_object_by_index(int index)
{
    SceneManager* sceneM = ServiceLocator::get<SceneManager>().get();

    return sceneM->get_current_scene()->get_entity_by_index(index);
}
