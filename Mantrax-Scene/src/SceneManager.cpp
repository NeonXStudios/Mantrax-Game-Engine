#include "../includes/SceneManager.h"

#include <iostream>
#include <string>
#include <vector>
#include <VarVerify.h>
#include <RenderPipeline.h>

using namespace std;

SceneManager *SceneManager::instance = nullptr;
bool SceneManager::loading_new_scene = false;

void SceneManager::on_awake()
{
    start_physic_world();

    for (Scene *scn : SceneManager::get_scene_manager()->opened_scenes)
    {
        scn->awake();
    }
}

void SceneManager::on_start()
{
    for (Scene *scn : SceneManager::get_scene_manager()->opened_scenes)
    {
        scn->init();
    }
}

void SceneManager::on_update()
{
    if (loading_new_scene == false)
    {
        SceneManager::get_scene_manager()->physic_world->update_world_physics();

        for (Scene *scn : SceneManager::get_scene_manager()->opened_scenes)
        {
            scn->update(Timer::delta_time);
        }
    }
}

void SceneManager::on_edition_mode()
{
    for (Scene *scn : SceneManager::get_scene_manager()->opened_scenes)
    {
        scn->on_edition_mode(Timer::delta_time);
    }
}

void SceneManager::on_clean_scenes()
{
    for (Scene *scn : SceneManager::get_scene_manager()->opened_scenes)
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

SceneManager *SceneManager::get_scene_manager()
{
    return instance;
}

string *SceneManager::get_open_scene_name()
{
    return &SceneManager::get_current_scene()->scene_name;
}

void SceneManager::clear_open_scene()
{
    for (Entity *g : SceneManager::get_current_scene()->objects_worlds)
    {
        g->ClearAllComponentes();
    }

    SceneManager::get_current_scene()->objects_worlds.clear();
}

void SceneManager::link_scene(Scene *scene_to_link)
{
    SceneManager::get_scene_manager()->opened_scenes.push_back(scene_to_link);
}

void SceneManager::load_scene_wrapped(std::string scene_name_new, bool is_additive)
{
    SceneManager::get_scene_manager()->load_scene(scene_name_new, is_additive);
}

Scene* SceneManager::load_scene(std::string scene_name_new, bool is_additive, std::string extension)
{
    loading_new_scene = true;
    SceneManager::get_scene_manager()->physic_world->collision_events->locked = true;


    if (!is_additive && SceneManager::get_scene_manager()->opened_scenes.size() > 0)
    {
        for (Scene *scn : SceneManager::get_scene_manager()->opened_scenes)
        {
            scn->clean_scene();
        }

        SceneManager::get_scene_manager()->opened_scenes.clear();
    }

    Scene *new_scene = SceneManager::make_new_empty_scene(scene_name_new);
    new_scene->unload_scene = true;

    std::string file_path = "";
    std::string assets_path = FileManager::get_project_path() + "assets/";

    for (std::string file_getted : FileManager::get_files_by_extension(assets_path, extension))
    {
        if (FileManager::get_file_info(file_getted).file_stem == scene_name_new)
        {
            file_path = file_getted;
            break;
        }
    }

    try
    {
        std::ifstream file(file_path);

        if (!file.is_open())
        {
            if (!std::filesystem::exists(file_path))
            {
                std::cerr << "File does not exist: " << file_path << std::endl;
            }
            std::cerr << "Error opening file creating new file for scene: " << file_path << std::endl;
        }

        json data_loaded;

        if (file)
        {
            file >> data_loaded;

            if (!data_loaded.is_array())
            {
                std::cerr << "Error: data_loaded is not an array" << std::endl;
            }
        }

        for (size_t i = 0; i < data_loaded.size(); i++)
        {
            Entity *new_object = new_scene->make_entity();
            VarVerify::set_value_if_exists(data_loaded[i], "name", new_object->name_object);

            auto transform = new_object->get_transform();

            glm::vec3 localPosition;
            VarVerify::set_value_if_exists(data_loaded[i], "px", localPosition.x);
            VarVerify::set_value_if_exists(data_loaded[i], "py", localPosition.y);
            VarVerify::set_value_if_exists(data_loaded[i], "pz", localPosition.z);
            transform->setPositionLocal(localPosition);

            glm::quat localRotation;
            VarVerify::set_value_if_exists(data_loaded[i], "rx", localRotation.x);
            VarVerify::set_value_if_exists(data_loaded[i], "ry", localRotation.y);
            VarVerify::set_value_if_exists(data_loaded[i], "rz", localRotation.z);
            VarVerify::set_value_if_exists(data_loaded[i], "rw", localRotation.w);
            transform->setRotationLocal(localRotation);

            glm::vec3 localScale;
            VarVerify::set_value_if_exists(data_loaded[i], "sx", localScale.x);
            VarVerify::set_value_if_exists(data_loaded[i], "sy", localScale.y);
            VarVerify::set_value_if_exists(data_loaded[i], "sz", localScale.z);
            transform->setScaleLocal(localScale);

            VarVerify::set_value_if_exists(data_loaded[i], "tag", new_object->object_tag);

            int object_int_id = -1;
            VarVerify::set_value_if_exists(data_loaded[i], "object_id", object_int_id);
            VarVerify::set_value_if_exists(data_loaded[i], "layer", new_object->Layer);

            new_object->get_transform()->update();

            new_object->object_int_id = object_int_id;
            new_object->object_string_id = std::to_string(object_int_id);

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
                                cmpGet->set_var<std::string>(var_object["name"], (std::string)var_object["value"]);
                            }
                            else if (var_object["type"] == "float")
                            {
                                cmpGet->set_var<float>(var_object["name"], (float)var_object["value"]);
                            }
                            else if (var_object["type"] == "int")
                            {
                                cmpGet->set_var<int>(var_object["name"], (int)var_object["value"]);
                            }
                            else if (var_object["type"] == "bool")
                            {
                                cmpGet->set_var<bool>(var_object["name"], (bool)var_object["value"]);
                            }
                            else if (var_object["type"] == "v3")
                            {
                                glm::vec3 val;
                                val.x = var_object["value"][0].get<float>();
                                val.y = var_object["value"][1].get<float>();
                                val.z = var_object["value"][2].get<float>();

                                cmpGet->set_var<glm::vec3>(var_object["name"], (glm::vec3)val);
                            }
                        }
                    }
                }
            }
            else
            {
                std::cerr << "Warning: 'components' is missing or not an array in entity " << i << std::endl;
            }
        }

        for (size_t i = 0; i < data_loaded.size(); i++)
        {
            int object_int_id = -1;
            int parentID = -1;

            VarVerify::set_value_if_exists(data_loaded[i], "object_id", object_int_id);
            VarVerify::set_value_if_exists(data_loaded[i], "parent_id", parentID);

            if (object_int_id != 1 && parentID != -1)
            {
                Entity *new_object_child = new_scene->get_entity_by_id(object_int_id);
                Entity *new_object_parent = new_scene->get_entity_by_id(parentID);

                if (new_object_child && new_object_parent)
                {
                    new_object_child->get_transform()->attach_to(new_object_parent->transform_component, false);
                }
            }
        }

        new_scene->awake();

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

        new_scene->unload_scene = false;
        loading_new_scene = false;
        SceneManager::get_scene_manager()->physic_world->collision_events->locked = false;
        std::cout << "----------------> Already Loaded Scene" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }


    return new_scene;
}

Scene *SceneManager::make_new_empty_scene(std::string scene_name)
{
    Scene *scene_raw = new Scene();
    scene_raw->scene_name = scene_name;
    
    try
    {
        std::cout << "------Making new Scene" << std::endl;

        if (scene_raw->main_camera == nullptr)
        {

            scene_raw->main_camera = new Camera();
            scene_raw->main_camera->render_id = RenderPipeline::add_render_texture()->get_render();
            scene_raw->main_camera->use_projection = false;
            scene_raw->main_camera->update();
        }
        else
        {
            std::cout << "Camera is not null in new scene" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }


    SceneManager::get_scene_manager()->opened_scenes.push_back(scene_raw);
    return scene_raw;
}

Scene *SceneManager::get_current_scene()
{
    if (SceneManager::get_scene_manager()->opened_scenes.size() <= 0)
    {
        std::cout << "Error not existe scene" << std::endl;
        return SceneManager::make_new_empty_scene("Empty Scene");
    }

    return SceneManager::get_scene_manager()->opened_scenes[0];
}

void SceneManager::start_physic_world()
{
    std::cout << "******* Starting Physic World" << std::endl;

    if (SceneManager::get_scene_manager()->physic_world == nullptr)
    {
        SceneManager::get_scene_manager()->physic_world = new PhysicsEngine();
    }

    SceneManager::get_scene_manager()->physic_world->start_world_physics();

    std::cout << "Physic World Started" << std::endl;
}

Scene* SceneManager::get_parent_scene_from_object(Entity* object)
{
    for (Scene* _scene : SceneManager::get_scene_manager()->opened_scenes) {
        for (Entity* _ent : _scene->objects_worlds)
        {
            if (_ent == object) {
                return _scene;
            }
        }
    }

    return nullptr;
}

void SceneManager::close_scene(Scene* p_scene)
{
    p_scene->clean_scene();
    p_scene->clean_all_components();

    auto& opened_scenes = SceneManager::get_scene_manager()->opened_scenes;

    auto it = std::find(opened_scenes.begin(), opened_scenes.end(), p_scene);
    if (it != opened_scenes.end()) {
        opened_scenes.erase(it);
    }
}
