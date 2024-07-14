#pragma once
#include <GarinNatives.h>
#include <GarinComponents.h>
#include <GarinBehaviours.h>
#include "Core.h"
#include <sol/sol.hpp>

class GScriptLua;
class GARINLIBS_API GScriptLua : public Component
{
public:
    sol::state lua;

    void defines() override;
    void init() override;
    void update() override;
    void clean() override;
};
