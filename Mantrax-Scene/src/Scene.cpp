#include "../includes/Scene.h"

using namespace nlohmann;

void Scene::awake()
{
    std::cout << "World loaded" << std::endl;
    on_awake();
}

void Scene::init()
{
    on_start();
}

void Scene::update(float delta_time)
{
    if (unload_scene == false)
    {
        try
        {
            for (Entity *ent : objects_worlds)
            {
                try
                {
                    ent->update();
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << '\n';
                }

                if (ent->hasComponent<GCamera>() && AppSettings::is_playing)
                {
                    static int current_depth = -1;

                    if (std::any_cast<int>(ent->getComponent<GCamera>().variableMap["Depth"]) > current_depth)
                    {
                        main_camera = ent->getComponent<GCamera>().a_camera;
                        current_depth = std::any_cast<int>(ent->getComponent<GCamera>().variableMap["Depth"]);
                        std::cout << "Camera changed" << std::endl;
                    }
                }
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
    entity_maked->object_int_id = IDGenerator::generate_id();
    entity_maked->object_string_id = std::to_string(entity_maked->object_int_id);

    objects_worlds.push_back(entity_maked);

    return entity_maked;
}

Entity *Scene::get_entity_by_index(int index)
{
    Entity *object_get = objects_worlds[index];

    if (objects_worlds[index] == nullptr)
    {
        return nullptr;
    }

    return object_get;
}

Entity *Scene::get_entity_by_id(int id)
{
    for (Entity *ent : objects_worlds)
    {
        if (ent->object_int_id == id)
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
        if (ent->object_string_id == id)
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
        if (ent->name_object == name)
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

Entity *Scene::find_entity_by_id(int id)
{
    for (Entity *entity : objects_worlds)
    {
        if (entity->object_int_id == id)
        {
            return entity;
        }
    }
    return nullptr;
}

void Scene::clean_scene()
{
    for (Entity *ent : objects_worlds)
    {
        ent->ClearAllComponentes();
    }

    objects_worlds.clear();
}