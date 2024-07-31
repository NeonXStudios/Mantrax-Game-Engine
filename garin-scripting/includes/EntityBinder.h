#pragma once
#include "sol/sol.hpp"
#include <glm/glm.hpp>
#include <InputSystem.h>
#include <ECS.h>
#include "sol/sol.hpp"
#include <GScriptLua.h>
#include <SceneManager.h>
#include <Core.h>

class GScriptLua;
class GARINLIBS_API EntityBinder
{
public:
    static void BinderFunction(GScriptLua *script);

    static GScriptLua *find_script(const std::string &script_name);
    static std::string get_filename_without_extension(const std::string &path);
};
