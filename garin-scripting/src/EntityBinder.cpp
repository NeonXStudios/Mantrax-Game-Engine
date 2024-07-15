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
                                        "GetModel", &Entity::getComponent<ModelComponent>,
                                        "GetBody", &Entity::getComponent<GBody>);

    luaParent->lua.new_usertype<TransformComponent>("Transform",
                                                    "position", &TransformComponent::Position,
                                                    "rotation", &TransformComponent::rotation,
                                                    "scale", &TransformComponent::Scale);

    luaParent->lua["Scene"] = sol::make_object(luaParent->lua.lua_state(), SceneManager::GetSceneManager());
    luaParent->lua.new_usertype<SceneManager>("SceneManager",
                                              "GetObjectPerID", &SceneManager::GetObjectPerIndex,
                                              "ChangeScene", &SceneManager::LoadScene,
                                              "newEntity", &SceneManager::NewEntity,
                                              "Destroy", &SceneManager::Destroy);

    luaParent->lua.new_usertype<GBody>("RigidBody",
                                       "static", &GBody::add_impulse,
                                       "position", &GBody::get_body_position,
                                       "set_position", &GBody::set_position,
                                       "mass", sol::property([](GBody &self)
                                                             { return std::any_cast<float>(self.variableMap["mass"]); }, [](GBody &self, float value)
                                                             { self.variableMap["mass"] = value; }),
                                       "isStatic", sol::property([](GBody &self)
                                                                 { return std::any_cast<bool>(self.variableMap["isStatic"]); }, [](GBody &self, bool value)
                                                                 { self.variableMap["isStatic"] = value; }),
                                       "useGravity", sol::property([](GBody &self)
                                                                   { return std::any_cast<bool>(self.variableMap["useGravity"]); }, [](GBody &self, bool value)
                                                                   { self.variableMap["useGravity"] = value; }));
}