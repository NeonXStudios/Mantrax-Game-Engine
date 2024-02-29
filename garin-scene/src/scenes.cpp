#include "scenes.h"

Entity *scenes::make_entity()
{
    Entity *entity_maked = new Entity();
    std::cout << "New entity created" << std::endl;
    entity_maked->objectID = objects_worlds.size();
    objects_worlds.push_back(entity_maked);

    return entity_maked;
}

Entity *scenes::get_entity_by_index(int index)
{
    Entity *object_get = objects_worlds[index];

    if (objects_worlds[index] == nullptr)
    {
        std::cout << "Object index not found return nullptr" << std::endl;
        return nullptr;
    }

    return object_get;
}