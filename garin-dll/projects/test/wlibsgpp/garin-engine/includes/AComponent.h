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

    static std::string demangle(const char *mangled_name)
    {
        // Implementación básica para fines ilustrativos

        // Copia el nombre mangled para trabajar sobre él
        std::string demangled_name = mangled_name;

        // Aquí puedes aplicar las reglas específicas de MSVC para demangle el nombre
        // Por ejemplo, remover prefijos, sufijos, convertir códigos mangled, etc.

        // Implementación básica: remover prefijo de clase
        if (demangled_name.find("class ") == 0)
        {
            demangled_name = demangled_name.substr(6); // Remueve "class "
        }

        // Puedes agregar más lógica según tus necesidades

        return demangled_name;
    }
};