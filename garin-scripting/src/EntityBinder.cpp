#include "../includes/EntityBinder.h"
#include <GarinComponents.h>

void EntityBinder::BinderFunction(GScriptLua *luaParent)
{
    if (luaParent->entity == nullptr)
    {
        std::cout << "ENTITY ITS NULL" << std::endl;
    }
    std::cout << "ENTITY BINDER" << std::endl;

    luaParent->lua["self"] = sol::make_object(luaParent->lua.lua_state(), luaParent->entity);

    luaParent->lua.new_usertype<Entity>("Object",
                                        "name", &Entity::ObjectName,
                                        "tag", &Entity::ObjectTag,
                                        "id", &Entity::objectID,

                                        // GET COMPONENTS
                                        "transform", &Entity::get_transform,
                                        "GetAudioSource", &Entity::getComponent<GAudio>,
                                        "GetCollider", &Entity::getComponent<GCollision>,
                                        "GetMaterial", &Entity::getComponent<GMaterial>,
                                        "GetLuaScript", &Entity::getComponent<GScriptLua>,
                                        "GetCharacter", &Entity::getComponent<GCharacter>,
                                        "GetModel", &Entity::getComponent<ModelComponent>);

    luaParent->lua["Scene"] = sol::make_object(luaParent->lua.lua_state(), SceneManager::GetSceneManager());
    luaParent->lua.new_usertype<SceneManager>("SceneManager",
                                              "GetObjectPerID", &SceneManager::GetObjectPerIndex,
                                              "ChangeScene", &SceneManager::LoadScene,
                                              "newEntity", &SceneManager::NewEntity,
                                              "Destroy", &SceneManager::Destroy);
}