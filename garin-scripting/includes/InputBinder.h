#pragma once
#include "sol/sol.hpp"
#include <glm/glm.hpp>
#include <InputSystem.h>
#include "sol/sol.hpp"
#include <GScriptLua.h>
#include <Core.h>

class GScriptLua;
class GARINLIBS_API InputBinder
{
public:
    static void BinderFunction(GScriptLua *script);
};
