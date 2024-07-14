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
class GARINLIBS_API EngineBinder
{
public:
    static void BinderFunction(GScriptLua *script);
};
