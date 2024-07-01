#pragma once
#include "AComponent.h"
#include "CModel.h"
#include "CRigidbody.h"
#include "CCollider.h"
#include "CGScript.h"
#include "CMaterial.h"
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
            // auto components = owner->getComponents<T>();
            // for (auto &cmap : components)
            // {
            //     ImGui::PushID(id);

            //     // Crear un TreeNode con el nombre del componente
            //     std::string componentName = demangle(typeid(T).name());

            //     // Dibujar el checkbox sin texto
            //     bool enabledCTMP = cmap->enabled;
            //     ImGui::Checkbox("", &enabledCTMP);
            //     cmap->enabled = enabledCTMP;

            //     // Mover a la misma línea para el TreeNode
            //     ImGui::SameLine();
            //     bool treeNodeOpen = ImGui::TreeNodeEx(componentName.c_str(), ImGuiTreeNodeFlags_SpanAvailWidth);

            //     // Colocar el botón "X" a la derecha del TreeNode
            //     ImGui::SameLine(ImGui::GetContentRegionMax().x - 20); // Ajusta el valor para alinear correctamente el botón "X"
            //     if (ImGui::Button("X"))
            //     {
            //         // if (std::is_same<T, SpriteComponent>::value)
            //         // {
            //         //     std::cout << "El componente base no puede ser eliminado" << std::endl;
            //         // }
            //         // else
            //         // {
            //         //     owner->removeComponent<T>(); // Adapt this to remove specific component
            //         // }
            //     }

            //     if (treeNodeOpen)
            //     {
            //         cmp->draw(owner);
            //         ImGui::Separator();

            //         ImGui::TreePop();
            //     }

            //     ImGui::PopID();
            //     id++; // Increment ID for each component of the same type
            // }

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
