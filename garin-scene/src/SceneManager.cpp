#include "../includes/SceneManager.h"

#include <iostream>
#include <string>
#include <vector>

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
