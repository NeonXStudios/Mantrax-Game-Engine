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

    GScriptLua *find_script(const std::string &script_name);
    std::string get_filename_without_extension(const std::string &path);

    template <typename... Args>
    void ExecuteFunction(const std::string &function_name, Args... args)
    {
        sol::function func = lua[function_name];
        if (func.valid())
        {
            func(args...);
        }
        else
        {
            std::cerr << "Function " << function_name << " not found in script" << std::endl;
        }
    }
};