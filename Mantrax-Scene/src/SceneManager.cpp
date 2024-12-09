#include "../includes/SceneManager.h"

#include <iostream>
#include <string>
#include <vector>
#include <VarVerify.h>

using namespace std;

SceneManager *SceneManager::instance = nullptr;

void SceneManager::on_awake()
{
    SceneManager::start_physic_world();

    for (Scene *scn : SceneManager::GetSceneManager()->opened_scenes)
    {
        scn->awake();
    }
}

void SceneManager::on_start()
{
    for (Scene *scn : SceneManager::GetSceneManager()->opened_scenes)
    {
        scn->on_start();
    }
}

void SceneManager::on_update()
{
    SceneManager::GetSceneManager()->physic_world->update_world_physics();

    for (Scene *scn : SceneManager::GetSceneManager()->opened_scenes)
    {
        scn->on_update(Timer::delta_time);
    }
}

void SceneManager::on_edition_mode()
{
    for (Scene *scn : SceneManager::GetSceneManager()->opened_scenes)
    {
        scn->on_edition_mode(Timer::delta_time);
    }
}

void SceneManager::on_clean_scenes()
{
    for (Scene *scn : SceneManager::GetSceneManager()->opened_scenes)
    {
        scn->on_destroy();
    }
}

void SceneManager::create()
{
    if (SceneManager::instance)
        throw std::exception("SceneManager already created.");

    SceneManager::instance = new SceneManager();
    std::cout << "SceneManager Created" << endl;
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

Entity *SceneManager::GetObjectPerTag(std::string tag)
{
    for (Entity *ent : SceneManager::GetOpenScene()->objects_worlds)
    {
        if (ent->ObjectTag == tag)
        {
            return ent;
        }
    }

    return nullptr;
}

Entity *SceneManager::GetObjectPerName(std::string name)
{
    for (Entity *ent : SceneManager::GetOpenScene()->objects_worlds)
    {
        if (ent->ObjectName == name)
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
        auto it = std::find(SceneManager::get_current_scene()->objects_worlds.begin(), SceneManager::get_current_scene()->objects_worlds.end(), obj);
        if (it != SceneManager::get_current_scene()->objects_worlds.end())
        {
            obj->ClearAllComponentes();
            SceneManager::get_current_scene()->objects_worlds.erase(it);
        }
    }
    return nullptr;
}

Entity *SceneManager::GetObjectPerIndex(int index)
{
    return SceneManager::get_current_scene()->objects_worlds[index];
}

Scene *SceneManager::GetOpenScene()
{
    return SceneManager::get_current_scene();
}

string *SceneManager::GetOpenSceneName()
{
    return &SceneManager::get_current_scene()->scene_name;
}

Entity *SceneManager::CloneEntity(Entity *entity)
{
    std::cout << "Components to clone: " << entity->GetAllComponent().size() << std::endl;
    return entity;
}

void SceneManager::ClearOpenScene()
{
    for (Entity *g : SceneManager::get_current_scene()->objects_worlds)
    {
        g->ClearAllComponentes();
    }

    SceneManager::get_current_scene()->objects_worlds.clear();
}

void SceneManager::link_scene(Scene *scene_to_link)
{
    SceneManager::GetSceneManager()->opened_scenes.push_back(scene_to_link);
}

void SceneManager::load_scene(std::string scene_name_new, bool is_additive)
{

    if (!is_additive && SceneManager::GetSceneManager()->opened_scenes.size() > 0)
    {
        SceneManager::get_current_scene()->clean_all_components();

        for (Scene *scn : SceneManager::GetSceneManager()->opened_scenes)
        {
            scn->on_destroy();
        }

        SceneManager::GetSceneManager()->opened_scenes.clear();
    }

    Scene *new_scene = new Scene();
    new_scene->unload_scene = true;

    std::string file_path = "";
    std::string assets_path = FileManager::get_project_path() + "assets/";

    for (std::string file_getted : FileManager::get_files_by_extension(assets_path, ".scene"))
    {
        if (FileManager::get_file_info(file_getted).file_stem == scene_name_new)
        {
            file_path = file_getted;
            break;
        }
    }

    try
    {
        new_scene->objects_worlds.clear();

        std::ifstream file(file_path);

        if (!file.is_open())
        {
            if (!std::filesystem::exists(file_path))
            {
                std::cerr << "File does not exist: " << file_path << std::endl;
                return;
            }

            new_scene->scene_name = "Error Scene";
            std::cerr << "Error opening file: " << file_path << std::endl;
            return;
        }

        json data_loaded;
        file >> data_loaded;

        if (!data_loaded.is_array())
        {
            std::cerr << "Error: data_loaded is not an array" << std::endl;
            return;
        }

        for (size_t i = 0; i < data_loaded.size(); i++)
        {
            Entity *new_object = new_scene->make_entity();
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

            VarVerify::set_value_if_exists(data_loaded[i], "tag", new_object->ObjectTag);

            int objectID = -1;

            VarVerify::set_value_if_exists(data_loaded[i], "object_id", objectID);
            VarVerify::set_value_if_exists(data_loaded[i], "layer", new_object->Layer);

            new_object->objectID = objectID;
            new_object->ObjectSTRID = std::to_string(objectID);

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
                            else if (var_object["type"] == "v3")
                            {
                                glm::vec3 val;
                                val.x = var_object["value"][0].get<float>();
                                val.y = var_object["value"][1].get<float>();
                                val.z = var_object["value"][2].get<float>();

                                cmpGet->setup_var(var_object["name"], val);
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

        for (size_t i = 0; i < data_loaded.size(); i++)
        {
            int objectID = -1;
            int parentID = -1;

            VarVerify::set_value_if_exists(data_loaded[i], "object_id", objectID);
            VarVerify::set_value_if_exists(data_loaded[i], "parent_id", parentID);

            if (objectID != 1 && parentID != -1)
            {
                Entity *new_object_child = new_scene->get_entity_by_id(objectID);
                Entity *new_object_parent = new_scene->get_entity_by_id(parentID);
                new_object_child->get_transform()->parent = new_object_parent->get_transform();

                new_object_parent->get_transform()->childrens.push_back(new_object_child->get_transform());
            }
        }

        try
        {
            for (Entity *ent : new_scene->objects_worlds)
            {
                for (Component *comp : ent->GetAllComponent())
                {
                    if (typeid(*comp) != typeid(GScript))
                    {
                        comp->init();
                    }
                }
            }

            for (Entity *ent : new_scene->objects_worlds)
            {
                for (Component *comp : ent->GetAllComponent())
                {
                    if (typeid(*comp) == typeid(GScript))
                    {
                        comp->init();
                    }
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }

        if (new_scene != nullptr)
        {
            SceneManager::GetSceneManager()->opened_scenes.push_back(new_scene);
            new_scene->scene_name = scene_name_new;
        }

        new_scene->unload_scene = false;
        std::cout << "----------------> Already Loade Scene" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

Scene *SceneManager::get_current_scene()
{
    return SceneManager::GetSceneManager()->opened_scenes[0];
}

void SceneManager::start_physic_world()
{

    if (SceneManager::GetSceneManager()->physic_world == nullptr)
    {
        SceneManager::GetSceneManager()->physic_world = new PhysicsEngine();
    }

    SceneManager::GetSceneManager()->physic_world->delete_phys_world();

    SceneManager::GetSceneManager()->physic_world->start_world_physics();

    std::cout << "Physic World Started" << std::endl;
}