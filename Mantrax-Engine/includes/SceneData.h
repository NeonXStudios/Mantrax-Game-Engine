#pragma once
#include <SceneManager.h>
#include "VarVerify.h"
#include <Core.h>

class GARINLIBS_API SceneData
{
public:
    static void load_scene(std::string scene_name, bool is_additive = false);
    static void save_scene(Scene* current_scene, std::string extension = ".scene");
    static void save_entitie(Scene* current_scene);

    static std::string demangle(const char *mangled_name)
    {
        std::string demangled_name = mangled_name;

        if (demangled_name.find("class ") == 0)
        {
            demangled_name = demangled_name.substr(6);
        }

        return demangled_name;
    }
};
