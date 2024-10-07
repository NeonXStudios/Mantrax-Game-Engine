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
                                        "layer", &Entity::Layer,
                                        "transform", &Entity::get_transform,
                                        "GetAudioSource", &Entity::getComponent<GAudio>,
                                        "GetCollider", &Entity::getComponent<GCollision>,
                                        "GetMaterial", &Entity::getComponent<GMaterial>,
                                        "GetScript", &Entity::getComponent<GScriptLua>,
                                        "GetCharacter", &Entity::getComponent<GCharacter>,
                                        "GetModel", &Entity::getComponent<ModelComponent>,
                                        "GetBody", &Entity::getComponent<GBody>,
                                        "GetAnimator", &Entity::getComponent<GAnimator>,
                                        "GetCharacterController", &Entity::getComponent<GCharacter>);

    luaParent->lua.new_usertype<GScriptLua>("GScriptLua",
                                            "ExecuteFunction", [](GScriptLua *self, const std::string &function_name, sol::variadic_args args)
                                            { self->ExecuteFunction(function_name, args); });

    luaParent->lua["FindScript"] = [](std::string script_name)
    { return find_script(script_name); };

    luaParent->lua.new_usertype<GAnimator>("Animator",
                                           "CurrentState", &GAnimator::current_state,
                                           "SetState", &GAnimator::set_state);

    luaParent->lua.new_usertype<TransformComponent>("Transform",
                                                    "position", &TransformComponent::Position,
                                                    "rotation", &TransformComponent::rotation,
                                                    "scale", &TransformComponent::Scale,
                                                    "attachTo", &TransformComponent::attachTo);

    luaParent->lua["Scene"] = sol::make_object(luaParent->lua.lua_state(), SceneManager::GetSceneManager());
    luaParent->lua.new_usertype<SceneManager>("SceneManager",
                                              "GetObjectPerID", &SceneManager::GetObjectByID,
                                              "GetObjectPerIndex", &SceneManager::GetObjectPerIndex,
                                              "GetObjectPerName", &SceneManager::GetObjectPerName,
                                              "GetObjectPerTag", &SceneManager::GetObjectPerTag,
                                              "ChangeScene", &SceneManager::LoadScene,
                                              "newEntity", &SceneManager::NewEntity,
                                              "Destroy", &SceneManager::Destroy);

    luaParent->lua.new_usertype<GCaster>("GCaster",
                                         "LineCast", [](glm::vec3 RayOrigin, glm::vec3 RayDirection, float Length, GCastHit &hit, unsigned int layerMask)
                                         { return GCaster::LineCast(RayOrigin, RayDirection, Length, &hit, layerMask); });

    luaParent->lua.new_usertype<GCastHit>("GCastHit",
                                          "point", &GCastHit::point,
                                          "normal", &GCastHit::normal,
                                          "entity", &GCastHit::entity);

    luaParent->lua.new_usertype<Timer>("Time",
                                       "DeltaTime", []()
                                       { return Timer::delta_time; });

    luaParent->lua["LAYER_0"] = LAYER_0;
    luaParent->lua["LAYER_1"] = LAYER_1;
    luaParent->lua["LAYER_2"] = LAYER_2;
    luaParent->lua["LAYER_3"] = LAYER_3;
    luaParent->lua["LAYER_4"] = LAYER_4;
    luaParent->lua["LAYER_5"] = LAYER_5;
    luaParent->lua["LAYER_6"] = LAYER_6;
    luaParent->lua["LAYER_7"] = LAYER_7;
    luaParent->lua["LAYER_8"] = LAYER_8;
    luaParent->lua["LAYER_9"] = LAYER_9;
    luaParent->lua["LAYER_10"] = LAYER_10;
    luaParent->lua["LAYER_11"] = LAYER_11;
    luaParent->lua["LAYER_12"] = LAYER_12;
    luaParent->lua["LAYER_13"] = LAYER_13;
    luaParent->lua["LAYER_14"] = LAYER_14;
    luaParent->lua["LAYER_15"] = LAYER_15;
    luaParent->lua["LAYER_16"] = LAYER_16;
    luaParent->lua["LAYER_17"] = LAYER_17;
    luaParent->lua["LAYER_18"] = LAYER_18;
    luaParent->lua["LAYER_19"] = LAYER_19;

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

GScriptLua *EntityBinder::find_script(const std::string &script_name)
{
    for (Entity *script_get : SceneManager::GetOpenScene()->objects_worlds)
    {
        for (GScriptLua *get_script : script_get->getComponents<GScriptLua>())
        {
            if (get_script != nullptr)
            {
                std::string script_path = std::any_cast<std::string>(get_script->variableMap["ScriptPath"]);
                std::string script_name_get = get_filename_without_extension(script_path);

                if (script_name_get == script_name)
                {
                    return get_script;
                }
            }
        }
    }
    return nullptr;
}

std::string EntityBinder::get_filename_without_extension(const std::string &path)
{
    std::filesystem::path fs_path(path);
    return fs_path.stem().string();
}

// TEMPLATE
//  luaParent->lua.new_usertype<GCharacter>("CharacterController",
//                                              "move", sol::property([](GBody &self)
//                                                                    { return std::any_cast<bool>(self.variableMap["useGravity"]); }, [](GBody &self, bool value)
//                                                                    { self.variableMap["useGravity"] = value; }));