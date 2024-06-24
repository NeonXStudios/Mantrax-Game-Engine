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

class AComponent
{
public:
    bool started = false;

    template <typename T>
    void update(Entity *owner, int id)
    {
        if (owner->hasComponent<T>())
        {
            ImGui::PushID(id);
            if (ImGui::Button("X"))
            {
                // if (std::is_same<T, SpriteComponent>::value)
                // {
                //     std::cout << "El componente base no puede ser eliminado" << std::endl;
                // }
                // else
                // {
                //     owner->removeComponent<T>();
                // }
            }
            else
            {
                draw(owner);
                ImGui::Separator();
            }
            ImGui::PopID();
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

    static std::string demangle(const char *name)
    {
        int status = -1;
        std::unique_ptr<char, void (*)(void *)> res{
            abi::__cxa_demangle(name, nullptr, nullptr, &status),
            std::free};
        return (status == 0) ? res.get() : name;
    }
};