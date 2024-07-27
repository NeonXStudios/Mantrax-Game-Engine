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
                                        "GetBody", &Entity::getComponent<GBody>,
                                        "GetCharacterController", &Entity::getComponent<GCharacter>);

    luaParent->lua.new_usertype<TransformComponent>("Transform",
                                                    "position", &TransformComponent::Position,
                                                    "rotation", &TransformComponent::rotation,
                                                    "scale", &TransformComponent::Scale,
                                                    "attachTo", &TransformComponent::attachTo);

    luaParent->lua["Scene"] = sol::make_object(luaParent->lua.lua_state(), SceneManager::GetSceneManager());
    luaParent->lua.new_usertype<SceneManager>("SceneManager",
                                              "GetObjectPerID", &SceneManager::GetObjectByID,
                                              "GetObjectPerIndex", &SceneManager::GetObjectPerIndex,
                                              "ChangeScene", &SceneManager::LoadScene,
                                              "newEntity", &SceneManager::NewEntity,
                                              "Destroy", &SceneManager::Destroy);

    luaParent->lua.new_usertype<GBody>("RigidBody",
                                       "position", &GBody::get_body_position,
                                       "set_position", &GBody::set_position,
                                       "impulse", &GBody::add_impulse,
                                       "mass", sol::property([](GBody &self)
                                                             { return std::any_cast<float>(self.variableMap["mass"]); }, [](GBody &self, float value)
                                                             { self.variableMap["mass"] = value; }),
                                       "isStatic", sol::property([](GBody &self)
                                                                 { return std::any_cast<bool>(self.variableMap["isStatic"]); }, [](GBody &self, bool value)
                                                                 { self.variableMap["isStatic"] = value; }),
                                       "useGravity", sol::property([](GBody &self)
                                                                   { return std::any_cast<bool>(self.variableMap["useGravity"]); }, [](GBody &self, bool value)
                                                                   { self.variableMap["useGravity"] = value; }));

    // luaParent->lua.new_usertype<GBodySpace::force_mode>("ForceMode",
    //                                                     "impulse", GBodySpace::force_mode::Impulse,
    //                                                     "velocity", GBodySpace::force_mode::Velocity,
    //                                                     "acceleration", GBodySpace::force_mode::Acceleration);

    luaParent->lua.new_usertype<GCharacter>("CharacterController",
                                            "move", &GCharacter::move);

    luaParent->lua.new_usertype<GAudio>("AudioSource",
                                        "Stop", &GAudio::Stop,
                                        "Reset", &GAudio::Reset,
                                        "SetPan", &GAudio::SetPan,
                                        "GetPan", &GAudio::GetPan,
                                        "SetVolumen", &GAudio::SetVolumen,
                                        "GetVolumen", &GAudio::GetVolumen,
                                        "IsPlaying", &GAudio::IsPlaying,
                                        "SetPauseState", &GAudio::SetPauseState,
                                        "MaxDistance", sol::property([](GAudio &self)
                                                                     { return std::any_cast<float>(self.variableMap["AudioMax"]); }, [](GAudio &self, float value)
                                                                     { self.variableMap["AudioMax"] = value; }),
                                        "MinDistance", sol::property([](GAudio &self)
                                                                     { return std::any_cast<float>(self.variableMap["AudioMin"]); }, [](GAudio &self, float value)
                                                                     { self.variableMap["AudioMin"] = value; }));
}

// TEMPLATE
//  luaParent->lua.new_usertype<GCharacter>("CharacterController",
//                                              "move", sol::property([](GBody &self)
//                                                                    { return std::any_cast<bool>(self.variableMap["useGravity"]); }, [](GBody &self, bool value)
//                                                                    { self.variableMap["useGravity"] = value; }));