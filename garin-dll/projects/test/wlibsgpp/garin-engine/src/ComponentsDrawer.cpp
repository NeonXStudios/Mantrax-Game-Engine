#include "../includes/ComponentsDrawer.h"
#include <type_traits>

// template <typename T>
// void ComponentsDrawer::updateUI(Entity *owner, int id, AComponent *cmp)
// {
//     if (owner->hasComponent<T>())
//     {
//         ImGui::PushID(id);
//         Component *cmap = &owner->getComponent<T>();
//         bool enabledCTMP = cmap->enabled;
//         ImGui::Checkbox("Enabled", &enabledCTMP);
//         cmap->enabled = enabledCTMP;

//         ImGui::SameLine();
//         if (ImGui::Button("X"))
//         {
//             // if (std::is_same<T, SpriteComponent>::value) {
//             //     std::cout << "El componente base no puede ser eliminado" << std::endl;
//             // } else {
//             //     owner->removeComponent<T>();
//             // }
//         }
//         else
//         {
//             cmp->draw(owner);
//             ImGui::Separator();
//         }

//         ImGui::PopID();
//     }
// }