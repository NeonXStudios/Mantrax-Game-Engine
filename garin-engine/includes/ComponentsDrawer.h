#pragma once
#include "AComponent.h"
#include "CModel.h"
#include <ECS.h>
#include <iostream>
#include <imgui.h>
#include <memory>
#include <cxxabi.h>

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

    static std::string demangle(const char *name)
    {
        int status = -1;
        std::unique_ptr<char, void (*)(void *)> res{
            abi::__cxa_demangle(name, nullptr, nullptr, &status),
            std::free};
        return (status == 0) ? res.get() : name;
    }
};
