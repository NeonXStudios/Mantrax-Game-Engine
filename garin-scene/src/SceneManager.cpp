#include "../includes/SceneManager.h"

#include <iostream>
#include <string>
#include <vector>
#include <VarVerify.h>

using namespace std;

SceneManager *SceneManager::instance = nullptr;

void SceneManager::create()
{
    if (SceneManager::instance)
        throw std::exception("SceneManager already created.");
    SceneManager::instance = new SceneManager();
    std::cout << "SceneManager Created" << endl;
    SceneManager::GetSceneManager()->OpenScene = new Scene();
}

void SceneManager::release()
{
    if (!SceneManager::instance)
        return;
    delete SceneManager::instance;
}

SceneManager *SceneManager::GetSceneManager()
{
    return instance;
}

Entity *SceneManager::NewEntity()
{
    Entity *entity_maked = new Entity();
    std::cout << "New entity created" << std::endl;
    entity_maked->objectID = IDGenerator::generate_id();
    entity_maked->ObjectSTRID = std::to_string(entity_maked->objectID);

    SceneManager::GetOpenScene()->objects_worlds.push_back(entity_maked);

    return entity_maked;
}

Entity *SceneManager::NewEntityNonSetupScene()
{
    Entity *entity_maked = new Entity();
    std::cout << "New entity created" << std::endl;
    entity_maked->objectID = IDGenerator::generate_id();
    entity_maked->ObjectSTRID = std::to_string(entity_maked->objectID);

    return entity_maked;
}

Entity *SceneManager::GetObjectByID(int id)
{
    for (Entity *ent : SceneManager::GetOpenScene()->objects_worlds)
    {
        if (ent->objectID == id)
        {
            return ent;
        }
    }

    return nullptr;
}

Entity *SceneManager::Destroy(Entity *obj)
{
    if (obj != nullptr)
    {
        auto it = std::find(OpenScene->objects_worlds.begin(), OpenScene->objects_worlds.end(), obj);
        if (it != OpenScene->objects_worlds.end())
        {
            obj->ClearAllComponentes();
            OpenScene->objects_worlds.erase(it);
        }
    }
    return nullptr;
}

Entity *SceneManager::GetObjectPerIndex(int index)
{
    return OpenScene->objects_worlds[index];
}

Scene *SceneManager::GetOpenScene()
{
    return SceneManager::instance->OpenScene;
}

string *SceneManager::GetOpenSceneName()
{
    return &SceneManager::GetSceneManager()->OpenScene->scene_name;
}

Entity *SceneManager::CloneEntity(Entity *entity)
{
    std::cout << "Components to clone: " << entity->GetAllComponent().size() << std::endl;
    return entity;
}

void SceneManager::ClearOpenScene()
{
    for (Entity *g : SceneManager::GetSceneManager()->OpenScene->objects_worlds)
    {
        g->ClearAllComponentes();
    }

    SceneManager::GetSceneManager()->OpenScene->objects_worlds.clear();
}

void SceneManager::LoadScene(string scenePath, string sceneName)
{
}

void SceneManager::load_scene(std::string scene_name_new)
{
    std::cout << "Starting load scene" << std::endl;
    std::string file_path = FileManager::get_execute_path() + "projects/test/assets/" + scene_name_new + ".scene";

    std::cout << "Loading scene from: " << file_path << std::endl;

    try
    {
        SceneManager::GetOpenScene()->unload_scene = true;

        if (SceneManager::GetOpenScene()->physic_world == nullptr)
        {
            SceneManager::GetOpenScene()->physic_world = new PhysicsEngine();
        }

        SceneManager::GetOpenScene()->physic_world->delete_phys_world();

        try
        {
            for (Entity *ent : SceneManager::GetOpenScene()->objects_worlds)
            {
                ent->ClearAllComponentes();
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }

        SceneManager::GetOpenScene()->physic_world->start_world_physics();

        SceneManager::GetOpenScene()->objects_worlds.clear();
        SceneManager::GetOpenScene()->scene_name = scene_name_new;

        std::ifstream file(file_path);
        if (!file.is_open())
        {
            std::cerr << "Error opening file: " << file_path << std::endl;
            return;
        }

        json data_loaded;
        file >> data_loaded;

        // Verificar que data_loaded sea un array
        if (!data_loaded.is_array())
        {
            std::cerr << "Error: data_loaded is not an array" << std::endl;
            return;
        }

        for (size_t i = 0; i < data_loaded.size(); i++)
        {
            Entity *new_object = SceneManager::GetOpenScene()->make_entity();
            VarVerify::set_value_if_exists(data_loaded[i], "name", new_object->ObjectName);

            auto transform = new_object->get_transform();
            VarVerify::set_value_if_exists(data_loaded[i], "px", transform->Position.x);
            VarVerify::set_value_if_exists(data_loaded[i], "py", transform->Position.y);
            VarVerify::set_value_if_exists(data_loaded[i], "pz", transform->Position.z);
            VarVerify::set_value_if_exists(data_loaded[i], "rx", transform->rotation.x);
            VarVerify::set_value_if_exists(data_loaded[i], "ry", transform->rotation.y);
            VarVerify::set_value_if_exists(data_loaded[i], "rz", transform->rotation.z);
            VarVerify::set_value_if_exists(data_loaded[i], "rw", transform->rotation.w);
            VarVerify::set_value_if_exists(data_loaded[i], "sx", transform->Scale.x);
            VarVerify::set_value_if_exists(data_loaded[i], "sy", transform->Scale.y);
            VarVerify::set_value_if_exists(data_loaded[i], "sz", transform->Scale.z);
            VarVerify::set_value_if_exists(data_loaded[i], "object_id", new_object->objectID);

            if (data_loaded[i].contains("components") && data_loaded[i]["components"].is_array())
            {
                json cmps = data_loaded[i]["components"];
                for (size_t e = 0; e < cmps.size(); e++)
                {
                    if (cmps[e].contains("name"))
                    {
                        std::string component_name = cmps[e]["name"];
                        Component *cmpGet = ComponentFactory::add_component(new_object, component_name);
                        json data = cmps[e]["varsdata"];

                        for (int i = 0; i < data.size(); i++)
                        {
                            json var_object = data[i];
                            if (var_object["type"] == "string")
                            {
                                cmpGet->setup_var(var_object["name"], (std::string)var_object["value"]);
                            }
                            else if (var_object["type"] == "float")
                            {
                                cmpGet->setup_var(var_object["name"], (float)var_object["value"]);
                            }
                            else if (var_object["type"] == "int")
                            {
                                cmpGet->setup_var(var_object["name"], (int)var_object["value"]);
                            }
                            else if (var_object["type"] == "bool")
                            {
                                cmpGet->setup_var(var_object["name"], (bool)var_object["value"]);
                            }
                        }
                    }
                }
            }
            else
            {
                std::cerr << "Warning: 'components' is missing or not an array in entity " << i << std::endl;
            }

            new_object->get_transform()->update();
        }

        // ATTACH WITH PARENTS
        for (size_t i = 0; i < data_loaded.size(); i++)
        {
            int objectID = -1;
            int parentID = -1;

            VarVerify::set_value_if_exists(data_loaded[i], "object_id", objectID);
            VarVerify::set_value_if_exists(data_loaded[i], "parent_id", parentID);

            if (objectID != 1 && parentID != -1)
            {
                Entity *new_object_child = SceneManager::GetOpenScene()->get_entity_by_id(objectID);
                Entity *new_object_parent = SceneManager::GetOpenScene()->get_entity_by_id(parentID);
                new_object_child->get_transform()->parent = new_object_parent->get_transform();

                new_object_parent->get_transform()->childrens.push_back(new_object_child->get_transform());
            }
        }

        for (Entity *ent : SceneManager::GetOpenScene()->objects_worlds)
        {
            for (Component *comp : ent->GetAllComponent())
            {
                comp->init();
            }
        }

        SceneManager::GetOpenScene()->unload_scene = false;
        std::cout << "Loaded Scene: " << scene_name_new << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
