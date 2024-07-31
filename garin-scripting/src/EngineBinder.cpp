#include "../includes/EngineBinder.h"
#include <GarinComponents.h>
#include <iostream>
#include "GMathfs.h"
#include <camera.h>

void EngineBinder::BinderFunction(GScriptLua *luaParent)
{

    // VECTORS
    luaParent->lua.new_usertype<glm::vec2>("vector2", sol::constructors<glm::vec2(float, float)>(),
                                           "x", &glm::vec2::x,
                                           "y", &glm::vec2::y);

    luaParent->lua["vector2"]["new"] = [](float x, float y)
    {
        return glm::vec2(x, y);
    };

    luaParent->lua.new_usertype<glm::vec3>("vector3", sol::constructors<glm::vec3(float, float, float)>(),
                                           "x", &glm::vec3::x,
                                           "y", &glm::vec3::y,
                                           "z", &glm::vec3::z);

    luaParent->lua["vector3"]["new"] = [](float x, float y, float z)
    {
        return glm::vec3(x, y, z);
    };

    luaParent->lua.new_usertype<glm::vec4>("vector4", sol::constructors<glm::vec4(float, float, float, float)>(),
                                           "x", &glm::vec4::x,
                                           "y", &glm::vec4::y,
                                           "z", &glm::vec4::z,
                                           "w", &glm::vec4::w);

    luaParent->lua["vector4"]["new"] = [](float x, float y, float z, float w)
    {
        return glm::vec4(x, y, z, w);
    };

    luaParent->lua.new_usertype<glm::quat>("quaternion", sol::constructors<glm::vec4(float, float, float, float)>(),
                                           "x", &glm::quat::x,
                                           "y", &glm::quat::y,
                                           "z", &glm::quat::z,
                                           "w", &glm::quat::w);

    luaParent->lua["quaternion"]["new"] = [](float x, float y, float z, float w)
    {
        return glm::quat(w, x, y, z);
    };

    // TIMER
    // luaParent->lua["GMathf"]["ClampFloat"] = [](float value, float min, float max)
    // { return GMathfs::ClampFloat(value, min, max); };
    // luaParent->lua["Time"]["DeltaTime"] = []()
    // { return Timer::delta_time; };

    std::cout << "ENGINE BINDER" << std::endl;

    if (SceneManager::GetSceneManager() == nullptr)
    {
        std::cout << "SCENE MANAGER ITS NULL" << std::endl;
    }

    if (SceneManager::GetSceneManager()->OpenScene->main_camera == nullptr)
    {
        std::cout << "SCENE CAMERA ITS NULL" << std::endl;
    }

    // GAME CAMERA
    luaParent->lua["GameCamera"] = sol::make_object(luaParent->lua.lua_state(), SceneManager::GetSceneManager()->OpenScene->main_camera);
    luaParent->lua.new_usertype<Camera>("Camera",
                                        "position", &Camera::cameraPosition,
                                        "fov", &Camera::zoom);

    luaParent->lua["GamePlaying"] = sol::make_object(luaParent->lua.lua_state(), AppSettings::is_playing);

    // DEBUG SETTINGS
    luaParent->lua.set_function("print", [](sol::variadic_args args)
                                {
            for (auto arg : args) {
                std::cout << arg.as <string>() << std::endl;
            }
            std::cout << std::endl; });

    luaParent->lua.set_function("mix_vec3", &GMathfs::mix);
    luaParent->lua.set_function("slerp_quat", &GMathfs::slerp);
    luaParent->lua.set_function("lerp_float", &GMathfs::lerp);

    std::cout << "ENGINE CORRECTLY BINDER" << std::endl;
}
