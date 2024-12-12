#pragma once
#include "../EngineUIBehaviour.h"
#include <iostream>
#include <ECS.h>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

struct ObjectMap
{
    std::string container_id;
    Entity *object;
};

struct FolderMap
{
    std::string container_id;
    std::string container_name;
    std::vector<ObjectMap *> objects_map = std::vector<ObjectMap *>();
};

class Hierarchy : public EngineUIBehaviour
{
public:
    bool right_click_held = false;
    bool show_script_popup = false;

    std::vector<FolderMap *> folders = std::vector<FolderMap *>();

    void remove_entity_from_all_folders(Entity *entity);
    bool is_descendant(Entity *potential_descendant, Entity *entity);

    void draw_entity_node(Entity *entity);
    bool is_in_any_folder(Entity *entity);
    void delete_folder(FolderMap *folder);
    void on_draw() override;

    void save_to_json(const std::string &file_path)
    {
        json j;

        for (const FolderMap *folder : folders)
        {
            json folder_json;
            folder_json["container_id"] = folder->container_id;
            folder_json["container_name"] = folder->container_name;

            json objects_json = json::array();
            for (const ObjectMap *obj : folder->objects_map)
            {
                objects_json.push_back({
                    {"object_name", obj->object->ObjectName},
                    {"object_id", obj->object->objectID},
                });
            }

            folder_json["objects"] = objects_json;
            j["folders"].push_back(folder_json);
        }

        std::ofstream file(file_path);
        file << j.dump(4);
    }

    void load_from_json(const std::string &file_path)
    {
        std::ifstream file(file_path);
        if (!file.is_open())
        {
            std::cerr << "No se pudo abrir el archivo JSON: " << file_path << std::endl;
            return;
        }

        json j;
        file >> j;

        folders.clear();

        Scene *current_scene = SceneManager::get_current_scene();
        if (!current_scene)
        {
            std::cerr << "No hay una escena activa para cargar los objetos." << std::endl;
            return;
        }

        for (const auto &folder_json : j["folders"])
        {
            FolderMap *folder = new FolderMap();
            folder->container_id = folder_json["container_id"].get<std::string>();
            folder->container_name = folder_json["container_name"].get<std::string>();

            for (const auto &object_json : folder_json["objects"])
            {
                int object_id = object_json["object_id"].get<int>();

                Entity *entity = current_scene->find_entity_by_id(object_id);
                if (entity)
                {
                    ObjectMap *obj = new ObjectMap();
                    obj->container_id = folder->container_id;
                    obj->object = entity;

                    folder->objects_map.push_back(obj);
                }
                else
                {
                    std::cerr << "No se encontró el objeto con ID: " << object_id << " en la escena." << std::endl;
                }
            }

            folders.push_back(folder);
        }
    }
};