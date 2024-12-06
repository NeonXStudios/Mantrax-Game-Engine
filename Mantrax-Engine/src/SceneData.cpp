#include "../includes/SceneData.h"

void SceneData::load_scene(std::string scene_name_new)
{
    SceneManager::load_scene(scene_name_new);
}

void SceneData::save_scene()
{
    try
    {
        json object;
        for (size_t i = 0; i < SceneManager::GetOpenScene()->objects_worlds.size(); i++)
        {
            Entity *ent = SceneManager::GetOpenScene()->objects_worlds[i];

            json dataOBJ;
            dataOBJ["name"] = ent->ObjectName;
            dataOBJ["px"] = ent->get_transform()->Position.x;
            dataOBJ["py"] = ent->get_transform()->Position.y;
            dataOBJ["pz"] = ent->get_transform()->Position.z;

            dataOBJ["rx"] = ent->get_transform()->rotation.x;
            dataOBJ["ry"] = ent->get_transform()->rotation.y;
            dataOBJ["rz"] = ent->get_transform()->rotation.z;
            dataOBJ["rw"] = ent->get_transform()->rotation.w;

            dataOBJ["sx"] = ent->get_transform()->Scale.x;
            dataOBJ["sy"] = ent->get_transform()->Scale.y;
            dataOBJ["sz"] = ent->get_transform()->Scale.z;

            dataOBJ["object_id"] = ent->objectID;
            dataOBJ["tag"] = ent->ObjectTag;
            dataOBJ["layer"] = ent->Layer;

            if (ent->get_transform()->parent != nullptr)
                dataOBJ["parent_id"] = ent->get_transform()->parent->entity->objectID;

            json components = json::array();

            std::vector<Component *> cmpms = ent->getComponents<Component>();

            for (Component *component : cmpms)
            {
                json comp;
                std::string className = demangle(typeid(*component).name());

                comp["name"] = className;
                json varsdata;

                for (const auto &[key, value] : component->variableMap)
                {
                    json datavar;
                    if (value.type() == typeid(std::string))
                    {
                        std::string val = std::any_cast<std::string>(value);
                        datavar["name"] = key;
                        datavar["value"] = val;
                        datavar["type"] = "string";
                    }
                    else if (value.type() == typeid(int))
                    {
                        int val = std::any_cast<int>(value);
                        datavar["name"] = key;
                        datavar["value"] = val;
                        datavar["type"] = "int";
                    }
                    else if (value.type() == typeid(float))
                    {
                        float val = std::any_cast<float>(value);
                        datavar["name"] = key;
                        datavar["value"] = val;
                        datavar["type"] = "float";
                    }
                    else if (value.type() == typeid(bool))
                    {
                        bool val = std::any_cast<bool>(value);
                        datavar["name"] = key;
                        datavar["value"] = val;
                        datavar["type"] = "bool";
                    }
                    else if (value.type() == typeid(glm::vec3))
                    {
                        glm::vec3 val = std::any_cast<glm::vec3>(value);
                        datavar["name"] = key;
                        datavar["value"] = {val.x, val.y, val.z};
                        datavar["type"] = "v3";
                    }

                    varsdata.push_back(datavar);
                }

                comp["varsdata"] = varsdata;
                components.push_back(comp);
            }

            dataOBJ["components"] = components;

            object.push_back(dataOBJ);
        }

        std::string file_path = FileManager::get_project_path() + "assets/" + SceneManager::GetOpenScene()->scene_name + ".scene";
        FileManager::write_file(file_path, object.dump(4));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
