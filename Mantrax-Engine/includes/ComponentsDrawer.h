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
#include <Core.h>

class GARINLIBS_API ComponentsDrawer
{
public:
    template <typename T>
    static void draw_ui(Entity *owner, int id, AComponent *cmp)
    {
        if (owner->hasComponent<T>())
        {
            auto modelComponents = owner->getComponents<T>();
            for (auto *cmap : modelComponents)
            {
                EditorGUI::Draw_Component<T>(owner, cmap, demangle(typeid(*cmap).name()),
                                             cmap->component_id, cmap, [cmap]() {});
            }
        }
    }

    static std::string demangle(const char *mangled_name)
    {
        std::string demangled_name = mangled_name;

        const std::string class_prefix = "class ";
        if (demangled_name.find(class_prefix) == 0)
        {
            demangled_name = demangled_name.substr(class_prefix.size());
        }

        const std::string struct_prefix = "struct ";
        if (demangled_name.find(struct_prefix) == 0)
        {
            demangled_name = demangled_name.substr(struct_prefix.size());
        }

        const std::string ptr64_suffix = " *__ptr64";
        size_t pos = demangled_name.find(ptr64_suffix);
        if (pos != std::string::npos)
        {
            demangled_name = demangled_name.substr(0, pos);
        }

        const std::string ptr_suffix = " *";
        pos = demangled_name.find(ptr_suffix);
        if (pos != std::string::npos)
        {
            demangled_name = demangled_name.substr(0, pos);
        }

        return demangled_name;
    }
};