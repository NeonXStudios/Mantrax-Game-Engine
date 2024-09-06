#pragma once
#include "EditorGUI.h"
#include <imgui.h>
#include <iostream>
#include <ECS.h>
#include <filesystem>
#include <string>
#include <vector>
#include <optional>
#include <ECS.h>
#include <sstream>
#include <cassert>
#include <Core.h>

class GARINLIBS_API AComponent
{
public:
    bool started = false;

    template <typename T>
    void update(Entity *owner, int id)
    {
        if (owner->hasComponent<T>())
        {
            draw(owner);
        }
    }

    virtual void start() {};
    virtual void draw(Entity *owner) {};

public:
    static string RemoveDir(string fullPath)
    {
        std::string targetFolder = "assets/";

        size_t pos = fullPath.find(targetFolder);
        if (pos != std::string::npos)
        {
            std::string extractedPath = fullPath.substr(pos + targetFolder.length());

            std::replace(extractedPath.begin(), extractedPath.end(), '/', '\\');

            return extractedPath;
        }

        return "";
    }

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