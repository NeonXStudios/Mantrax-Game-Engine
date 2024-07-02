#pragma once
#include "AComponent.h"
#include "CModel.h"
#include "CRigidbody.h"
#include "CCollider.h"
#include "CGScript.h"
#include "CMaterial.h"
#include "CCharacter.h"
#include <ECS.h>
#include <iostream>
#include <imgui.h>
#include <memory>
#include <sstream>

class ComponentsDrawer
{
public:
    template <typename T>
    static void updateUI(Entity *owner, int id, AComponent *cmp)
    {
        if (owner->hasComponent<T>())
        {
            cmp->draw(owner);
        }
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
