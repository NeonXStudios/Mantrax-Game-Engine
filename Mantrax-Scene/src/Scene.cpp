#include "../includes/Scene.h"

using namespace nlohmann;

void Scene::init()
{
    on_start();
}

void Scene::update(float delta_time)
{
    if (!unload_scene)
    {
        try
        {
            physic_world->update_world_physics();
            for (Entity *ent : objects_worlds)
            {
                ent->update();
            }

            on_update(delta_time);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

Entity *Scene::make_entity()
{
    Entity *entity_maked = new Entity();
    std::cout << "New entity created" << std::endl;
    entity_maked->objectID = IDGenerator::generate_id();
    entity_maked->ObjectSTRID = std::to_string(entity_maked->objectID);

    objects_worlds.push_back(entity_maked);

    return entity_maked;
}

Entity *Scene::get_entity_by_index(int index)
{
    Entity *object_get = objects_worlds[index];

    if (objects_worlds[index] == nullptr)
    {
        std::cout << "Object index not found return nullptr" << std::endl;
        return nullptr;
    }

    return object_get;
}

Entity *Scene::get_entity_by_id(int id)
{
    for (Entity *ent : objects_worlds)
    {
        if (ent->objectID == id)
        {
            return ent;
        }
    }

    return nullptr;
}

Entity *Scene::get_entity_by_id_string(std::string id)
{
    for (Entity *ent : objects_worlds)
    {
        if (ent->ObjectSTRID == id)
        {
            return ent;
        }
    }

    return nullptr;
}

Entity *Scene::get_entity_by_name(std::string name)
{
    for (Entity *ent : objects_worlds)
    {
        if (ent->ObjectName == name)
        {
            return ent;
        }
    }

    return nullptr;
}

void Scene::destroy(Entity *object_to_destroy)
{
    if (object_to_destroy != nullptr)
    {
        auto it = std::find(objects_worlds.begin(), objects_worlds.end(), object_to_destroy);
        if (it != objects_worlds.end())
        {
            object_to_destroy->ClearAllComponentes();
            objects_worlds.erase(it);
        }
    }
}